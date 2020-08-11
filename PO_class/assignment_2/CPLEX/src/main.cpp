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
    } // n X n

    IloArray < IloBoolVarArray > mode(env, n);
    for(int i = 0; i < n; i++){
        IloBoolVarArray array(env, N_WAYS);
        mode[i] = array;
    } // 6X3

    IloArray < IloNumVarArray > c(env,n); // o dia de termino

    IloVar alfa(env); //tempo de término ---> somatório das durações
    IloVar beta(env); //tempo de atraso ---> bounded var(lb = 0) b >= (a - 100)
    

    // 
    //  FO
    //  Soma dos custos + beta*multa
    IloExpr FO(env);

    for(int i = 0; i < n; i++){
    	for(int j = 0; j < N_WAYS; j++){
    		FO += mode[i][j] * days[i][j];
    	}
    }

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
            IloRange r = (Constraint1 == 1); // só se liga a um
            IloRange r = (Constraint1 == 1); // só recebe um
            model.add(r);

        }
   	}

    // CT3 -> precedência
    // CT4 -> C_j >= C_i + D_j - (1 - xij)*M
    // CT5 -> só pode ter um modo
    // CT6 -> alfa = sum D * modo
    // CT7 -> beta >= (alfa - 100) , lb = 0

    // env.end();
}