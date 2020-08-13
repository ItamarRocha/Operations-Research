#include "../include/Data.hpp"
#include <ilcplex/ilocplex.h>

void solve(Data *data);

int main(int argc, char* argv[]){

	if(argc != 2){
		std::cout << "Wrong Pattern\n./planning instances/archive\n";
		exit(1);
	}
	Data data(argv[1]);

	solve(&data);

	exit(0);
}

void solve(Data *data){

    int N_tasks = data->getNTasks(); // phantom node
    int deadline = data->getNDays();
    int penalty = data->getPenalty();
    int max_duration = data->getMaxDuration();

    //std::cout << n << " " << D << " " << penalty << " " << max_duration << std::endl;

    IloEnv env;
    IloModel model(env);
    //     
    // 2D array to store the decision variable x, that says the order, which one preceds the other  
    // nXn array
    //
    IloArray < IloBoolVarArray > x(env, N_tasks);
    for(int i = 0; i < N_tasks; i++){
        IloBoolVarArray array(env, N_tasks);
        x[i] = array;
    } // n X n

    for(int i = 0; i < N_tasks; i++){
        for(int j = 0; j < N_tasks; j++){
            char name[100];
            sprintf(name, "X[%d][%d]",i ,j);
            x[i][j].setName(name);
        }
    }


    IloArray < IloBoolVarArray > mode(env, N_tasks);
    for(int i = 0; i < N_tasks; i++){
        IloBoolVarArray array(env, N_MODES);
        mode[i] = array;
    } // 6X3

    for(int i = 0; i < N_tasks; i++){
        for(int k = 0; k < N_MODES; k++){
            char name[100];
            sprintf(name, "mode[%d][%d]",i ,k);
            mode[i][k].setName(name);
            //model.add(x[i][j]);
        }
    }

    IloNumVarArray C(env, N_tasks, 0, IloInfinity, ILOINT); // o dia de termino

    for(int i = 0; i < N_tasks; i++){
        char name[100];
        sprintf(name, "C[%d]",i);
        C[i].setName(name);
        //model.add(x[i][j]);
    
    }

    IloIntVar alfa(env); //tempo de término ---> somatório das durações
    alfa.setName("alfa");
    IloIntVar beta(env, 0, max_duration); //tempo de atraso ---> bounded var(lb = 0) b >= (a - 100)
    beta.setName("beta");
    // 
    //  FO
    //  Soma dos custos + beta*penalty
    IloExpr FO(env);

    for(int i = 0; i < N_tasks; i++){
    	for(int k = 0; k < N_MODES; k++){
    		FO += mode[i][k] * data->getCost(i,k);
    		//std::cout << data->getCost(i,k) << " ";
    	}
    }
    FO += beta*penalty;

    model.add(IloMinimize(env,FO));

	// //     
	// // Constraints
	// //

    // CT1 -> só sai um
    // CT2 -> só entra um
    for(int i = 0; i < N_tasks; i++){

        IloExpr Constraint1(env);
        IloExpr Constraint2(env);

        for(int j = 0; j < N_tasks; j++){
        	Constraint2 += x[j][i];
            Constraint1 += x[i][j];
        }
        IloRange r1 = (Constraint1 == 1); // só se liga a um
        IloRange r2 = (Constraint2 == 1); // só recebe um
        model.add(r1);
        model.add(r2);
   	}

    //CT3 -> C_j >= C_i + D_j - (1 - xij)*M
    for(int i = 0; i < N_tasks; i++){
        for(int j = 1; j < N_tasks; j++){
            if(i != j){
                int duration = 0;
                IloExpr Constraint4(env);
                for(int k = 0; k < N_MODES; k++){
                    Constraint4 += mode[j][k]* data->getDaysTaken(j,k);
                }
                Constraint4 += C[i];
                Constraint4 += -1*max_duration;
                Constraint4 += x[i][j] * max_duration;
                IloRange r = (Constraint4 - C[j] <= 0);
                model.add(r);
            }
        }
    }

    // CT4 Clossing the cicle and starting the duration of node 0 to 0
    // CT5 -> precedence
    // model.add(x[6][0] == 1);
    // model.add(x[0][1] == 1);
    model.add(C[0] == 0);
    
    int last = 0;
    bool add_first_connection = true;

    for(auto pair: data->getPrecedences()){
    	if(add_first_connection){
    		model.add(x[0][pair.first] == 1);
    		add_first_connection = false;
    	}
    	//std::cout << pair.first << " " << pair.second << std::endl;
    	model.add(C[pair.second] - C[pair.first] > 0);
    	last = pair.second;
    }
    model.add(x[last][0] == 1);

    // CT6 -> só pode ter um modo
    for(int i = 0; i < N_tasks; i++){
        IloExpr Constraint5(env);
        for(int k = 0; k < N_MODES; k++){
        	std::cout << i << " " << k << std::endl;
            Constraint5 += mode[i][k];
        }
        std::cout << std::endl;
        IloRange r = (Constraint5 == 1);
        model.add(r);
    }

    // CT7 -> alfa = sum deadline * modo
    IloExpr Constraint6(env);
    for(int i = 0; i < N_tasks; i++){
        for(int k = 0; k < N_MODES; k++){
            Constraint6 += data->getDaysTaken(i,k) * mode[i][k];    
        }
    }
    model.add(alfa == Constraint6);

    // CT8 -> beta >= (alfa - 100) , lb = 0
    IloExpr Constraint7(env);
    Constraint7 = alfa - deadline;
    model.add(beta - Constraint7 >= 0);

    // CT9 Não repetir tarefa // não se ligar a si mesmo
    for(int i = 0; i < N_tasks; i++){
        model.add(x[i][i] == 0); 
    } 


    IloCplex solver(model);
    solver.exportModel("model.lp");
    
    try{
        solver.solve();
    }catch(...){
        std::cout << "deu ruim" << std::endl;
    }
    // auto stop = high_resolution_clock::now();
    // auto duration = duration_cast<microseconds>(stop - start); 
      
    // std::cout << "duration = " << duration.count() << std::endl; 

    std::cout << "status:" << solver.getStatus() << std::endl;
    std::cout << "Objective function:" << solver.getObjValue() << std::endl;
    
    for(int i = 0; i < N_tasks; i++){
        std::cout << "Duration of task " << i << " = " << solver.getValue(C[i]) << std::endl;
    }
    for(int i = 0; i < N_tasks; i++){
        for(int j = 0; j < N_tasks; j++){
            int result_x = solver.getValue(x[i][j]);
            if(result_x)
                std::cout << "x[" << i << "][" << j << "] = " << result_x << std::endl;
        }     
    }
    for(int i = 0; i < N_tasks; i++){
        for(int k = 0; k < N_MODES; k++){
            int result_x = solver.getValue(mode[i][k]);
            if(result_x)
            	std::cout << "mode[" << i << "][" << k << "] = " << result_x << std::endl;
        }     
    }        
    env.end();
}