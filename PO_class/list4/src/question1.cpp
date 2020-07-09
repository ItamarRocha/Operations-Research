#include <iostream>
#include <ilcplex/ilocplex.h>
#define SIZE 20
#define VARIABLE_RANGE 9
#define SQUARE_SIDE 3
#define MAGIC_SQUARE_SUM 15
int main(){

	IloEnv env;
	IloModel model(env);


	IloArray < IloArray <IloBoolVarArray >> x(env,SQUARE_SIDE);
	// creating a triple matrix to the variables

	for(int i = 0; i < SQUARE_SIDE; i++){
		IloArray < IloBoolVarArray > array(env,SQUARE_SIDE);
		x[i] = array;
		for(int j = 0; j < SQUARE_SIDE; j++){
			IloBoolVarArray variables(env, VARIABLE_RANGE);
			x[i][j] = variables;

		}
	}

	for(int i = 0; i < SQUARE_SIDE; i++){
		for(int j = 0; j < SQUARE_SIDE; j++){
			for(int k = 0; k < VARIABLE_RANGE; k++){
				char name[SIZE];
				sprintf(name,"X[%d][%d][%d]",i,j,k);
				x[i][j][k].setName(name);
				model.add(x[i][j][k]);
			}
		}
	}
	//std::cout << x[2][2][8].getName() << std::endl;

	IloExpr FO(env); // empty objective function since we dont need it
					 // we can solve the problem just with the constraints

	model.add(IloMinimize(env,FO));

	// 	
	// CONSTRAINT 1 -> there can be only one number at each bool array
	// 
	for(int i = 0; i < SQUARE_SIDE; i++){
		for(int j = 0; j < SQUARE_SIDE; j++){
			IloExpr Constraint1(env);
			for(int k = 0; k < VARIABLE_RANGE; k++){
				Constraint1 += x[i][j][k];
			}
			IloRange r = (Constraint1 == 1);
			model.add(r);
		}
	}

	// 
	// CONSTRAINT 2 -> each number can only appear one time
	//

	for(int k = 0; k < VARIABLE_RANGE; k++){
		IloExpr Constraint2(env);
		for(int i = 0; i < SQUARE_SIDE; i++){
			for(int j = 0; j < SQUARE_SIDE; j++){
				Constraint2 += x[i][j][k];
			}
		}
		IloRange r = (Constraint2 == 1);
		model.add(r);
	}

	// 	
	// CONSTRAINT 3 -> sum over lines must be equal 15
	// 

	for(int i = 0; i < SQUARE_SIDE; i++){
		IloExpr Constraint3(env);
		for(int j = 0; j < SQUARE_SIDE; j++){
			for(int k = 0; k < VARIABLE_RANGE; k++){
				Constraint3 += x[i][j][k] * (k + 1);
			}
		}
		IloRange r = (Constraint3 == MAGIC_SQUARE_SUM);
		model.add(r);
	}

	// 
	// CONSTRAINT 4 -> sum over columns must be equal 15
	// 	

	for(int j = 0; j < SQUARE_SIDE; j++){
		IloExpr Constraint4(env);
		for(int i = 0; i < SQUARE_SIDE; i++){
			for(int k = 0; k < VARIABLE_RANGE; k++){
				Constraint4 += x[i][j][k] * (k + 1);
			}
		}
		IloRange r = (Constraint4 == MAGIC_SQUARE_SUM);
		model.add(r);
	}
	
	// 
	// CONSTRAINT 5 -> sum over first diagonal must be equal 15
	// CONSTRAINT 6 -> sum over secondary diagonal must be equal 15
	//

	IloExpr Constraint5(env);
	IloExpr Constraint6(env);

	for(int i = 0; i < SQUARE_SIDE; i++){
		for(int k = 0; k < VARIABLE_RANGE; k++){
			Constraint5 += x[i][i][k] * (k + 1);
			Constraint6 += x[i][SQUARE_SIDE - 1 - i][k] * (k + 1);
		}
	}
	IloRange r = (Constraint5 == MAGIC_SQUARE_SUM);
	IloRange r2 = (Constraint6 == MAGIC_SQUARE_SUM);
	model.add(r);
	model.add(r2);
		
	// 
	// SOLVING THE MODEL	
	// 	

	IloCplex cplex(model);

	try
	{
		cplex.solve();
	}
	catch(IloException& e)
	{
		std::cout << e;
	}

	std::cout << "status:" << cplex.getStatus() << std::endl;
	std::cout << "Z : " << cplex.getObjValue() << std::endl;
	for(int i = 0; i < SQUARE_SIDE; i++){
		for(int j = 0; j < SQUARE_SIDE; j++){
			for(int k = 0; k < VARIABLE_RANGE; k++){
				if(cplex.getValue(x[i][j][k]) > 0.00001)
					std::cout << " x["<< i <<"]["<< j <<"] : " << cplex.getValue(x[i][j][k])* (k+1);
			}
		}
		std::cout << std::endl;
	}
	cplex.exportModel("questao1.lp");
	env.end();

	exit(0);

}