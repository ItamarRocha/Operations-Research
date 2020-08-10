#include <ilcplex/ilocplex.h>
#include <chrono> 
#include <vector>
#include <iostream>
using namespace std::chrono;

#define N_WAYS 3

int main(){

    int n = 6;
    int D = 100;
    int M = 2000;

    std::vector<std::vector<int>> days = {{71, 60, 40},
                                        {55, 50, 44},
                                        {27, 23, 17},
                                        {31, 26, 20},
                                        {40, 35, 30},
                                        {10, 9, 8}};

    std::vector<std::vector<int>> price = {{0, 12000, 24000},
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
    }

    IloArray < IloBoolVarArray > mode(env, n);
    for(int i = 0; i < n; i++){
        IloBoolVarArray array(env, N_WAYS);
        mode[i] = array;
    }

    IloArray < IloNumVarArray > c(env,n);

    IloBoolVar isGreaterThanD(env);
    // 
    //  FO
    // 
    
    IloExpr FO(env);

    for(int i = 0; i < n; i++){
    	for(int j = 0; j < N_WAYS; j++){
    		FO += mode[i][j] * days[i][j];
    	}

    }

	//     
	// Constraints
	// 

    for(int i = 0; i < n; i++){

        IloExpr Constraint1(env);
        IloExpr Constraint2(env);

        for(int j = 0; j < n; j++){
        	Constraint2 += x[j][i];
            Constraint1 += x[i][j];
            if(j != n-1){
            	IloRange r = (Constraint1 == 1); // só se liga a um
            }else{
            	IloRange r = (Constraint1 == 0); // no final nao se liga a ning
            }
            IloRange r = (Constraint1 == 1); // só recebe um
            model.add(r);

        }
   	}
    // IloCplex mfp(model);

    // auto start = high_resolution_clock::now(); 

    // try{
    //     mfp.solve();
    // }catch(...){
    //     std::cout << "deu ruim" << std::endl;
    // }
    // auto stop = high_resolution_clock::now();
    // auto duration = duration_cast<microseconds>(stop - start); 
      
    // std::cout << "duration = " << duration.count() << std::endl; 

    // std::cout << "status:" << mfp.getStatus() << std::endl;
    // std::cout << "Objective function:" << mfp.getObjValue() << std::endl;
    // for(int i = 0; i < d1->getNVertex(); i++){
    //     for(int j = 0; j < d1->getNVertex(); j++){
    //         if(mfp.getValue(x[i][j]) > 0.01)
    //             std::cout << " X[" << i + 1 << "][" << j + 1 << "] = " << mfp.getValue(x[i][j]);
    //     }
    //     std::cout << std::endl;
    // }
    // // o fluxo maximo vai ser o maximo possivel menos o que passou pelo arco que tem custo
    // std::cout << "Max flow = " << max_flow - mfp.getObjValue() << std::endl; //remove the number of paths that went through the penalty paths


    // mfp.exportModel("model.lp");

    // env.end();
}