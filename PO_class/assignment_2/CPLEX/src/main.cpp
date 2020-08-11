#include <ilcplex/ilocplex.h>
#include <chrono> 
#include <vector>
#include <iostream>
using namespace std::chrono;

#define N_WAYS 3

int main(){

    int n = 7; // 6 + phantom
    int D = 100;
    int multa = 2000;
    int max_duration = 71 + 55 + 27 + 31 + 40 + 10;

    std::vector<std::vector<int>> days = {{0, 0, 0},
                                        {71, 60, 40},
                                        {55, 50, 44},
                                        {27, 23, 17},
                                        {31, 26, 20},
                                        {40, 35, 30},
                                        {10, 9, 8}};

    std::vector<std::vector<int>> price = {{0, 0, 0},
                                        {0, 12000, 24000},
                                        {0, 4000, 5000},
                                        {0, 13000, 17000},
                                        {0, 10000, 19000},
                                        {0, 5000, 7000},
                                        {0, 1000, 6000}};


    std::cout << days[5][2] << std::endl;
    IloEnv env;
    IloModel model(env);
    //     
    // 2D array to store the decision variable x, that says the order, which one preceds the other  
    // nXn array
    //
    IloArray < IloBoolVarArray > x(env, n);
    for(int i = 0; i < n; i++){
        IloBoolVarArray array(env, n);
        x[i] = array;
    } // n X n

    IloArray < IloBoolVarArray > mode(env, n);
    for(int i = 0; i < n; i++){
        IloBoolVarArray array(env, N_WAYS);
        mode[i] = array;
    } // 6X3

    IloNumVarArray C(env, n, 0, IloInfinity, ILOINT); // o dia de termino

    IloIntVar alfa(env); //tempo de término ---> somatório das durações
    IloIntVar beta(env, 0, max_duration); //tempo de atraso ---> bounded var(lb = 0) b >= (a - 100)

    // 
    //  FO
    //  Soma dos custos + beta*multa
    IloExpr FO(env);

    for(int i = 0; i < n; i++){
    	for(int j = 0; j < N_WAYS; j++){
    		FO += mode[i][j] * price[i][j];
    	}
    }
    FO += beta*multa;

    model.add(IloMinimize(env,FO));

	//     
	// Constraints
	//

    // CT1 -> só sai um
    // CT2 -> só entra um
    for(int i = 0; i < n; i++){

        IloExpr Constraint1(env);
        IloExpr Constraint2(env);

        for(int j = 0; j < n; j++){
        	Constraint2 += x[j][i];
            Constraint1 += x[i][j];
            IloRange r1 = (Constraint1 == 1); // só se liga a um
            IloRange r2 = (Constraint2 == 1); // só recebe um
            model.add(r1);
            model.add(r2);

        }
   	}

    // CT3 -> C_j >= C_i + D_j - (1 - xij)*M
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            int duration = 0;
            IloExpr Constraint4(env);
            for(int k = 0; k < N_WAYS; k++){
                Constraint4 += mode[j][k]*days[j][k];
            }
            Constraint4 += C[i] - C[j];
            Constraint4 += -(1 - x[i][j])*max_duration;
            IloRange r = (Constraint4 <= 0);
            model.add(r);
        }
    }
    // CT4 -> precedência
    model.add(C[3] > C[1]);
    model.add(C[3] > C[2]);
    model.add(C[4] > C[3]);
    model.add(C[5] > C[3]);
    model.add(C[6] > C[4]);
    model.add(C[6] > C[5]);

    // CT5 -> só pode ter um modo
    for(int i = 0; i < n; i++){
        IloExpr Constraint5(env);
        for(int k = 0; k < N_WAYS; k++){
            Constraint5 += mode[i][k];
        }
        IloRange r = (Constraint5 == 1);
        model.add(r);
    }
    // CT6 -> alfa = sum D * modo
    IloExpr Constraint6(env);
    for(int i = 0; i < n; i++){
        for(int k = 0; k < N_WAYS; k++){
            Constraint6 += days[i][k] * mode[i][k];    
        }
    }
    model.add(alfa == Constraint6);
    // CT7 -> beta >= (alfa - 100) , lb = 0
    IloExpr Constraint7(env);
    Constraint7 += alfa - 100;
    model.add(beta >= Constraint7);



    IloCplex solver(model);

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
    // for(int i = 0; i < d1->getNVertex(); i++){
    //     for(int j = 0; j < d1->getNVertex(); j++){
    //         if(solver.getValue(x[i][j]) > 0.01)
    //             std::cout << " X[" << i + 1 << "][" << j + 1 << "] = " << solver.getValue(x[i][j]);
    //     }
    //     std::cout << std::endl;
    // }
    // o fluxo maximo vai ser o maximo possivel menos o que passou pelo arco que tem custo
    //std::cout << "Max flow = " << max_flow - solver.getObjValue() << std::endl; //remove the number of paths that went through the penalty paths


    solver.exportModel("model.lp");

    env.end();
}