#include <iostream>
#include <ilcplex/ilocplex.h>
#define SIZE 20
#define VARIABLE_RANGE 9
#define SQUARE_SIDE 3
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




	/*for(int i = 0; i < SQUARE_SIDE; i++){
		IloExpr Constraint1(env); // PEGA A SOMA DAS LINHAS
		IloExpr Constraint2(env); // PEGA A SOMA DAS COLUNAS
		
		for(int j = 0; i < SQUARE_SIDE; i++){
			Constraint1 += x[i][j];		
			Constraint2 += x[j][i];
		}
		IloRange r1 = (Constraint1 == 15);
		IloRange r2 = (Constraint2 == 15);
		model.add(r1);
		model.add(r2);
	}*/

/*	IloExpr Constraint1(env);
	Constraint1 = x[0][0] + x[0][1] + x[0][2];
	IloRange r1 = (Constraint1 == 15);
	model.add(r1);

	IloExpr Constraint2(env);
	Constraint2 = x[1][0] + x[1][1] + x[1][2];
	IloRange r2 = (Constraint2 == 15);
	model.add(r2);

	IloExpr Constraint5(env);
	Constraint5 = x[2][0] + x[2][1] + x[2][2];
	IloRange r5 = (Constraint5 == 15);
	model.add(r5);

	IloExpr Constraint6(env);
	Constraint6 = x[0][0] + x[1][0] + x[2][0];
	IloRange r6 = (Constraint6 == 15);
	model.add(r6);

	IloExpr Constraint7(env);
	Constraint7 = x[0][1] + x[1][1] + x[2][1];
	IloRange r7 = (Constraint7 == 15);
	model.add(r7);

	IloExpr Constraint8(env);
	Constraint8 = x[0][2] + x[1][2] + x[2][2];
	IloRange r8 = (Constraint8 == 15);
	model.add(r8);
*/
	// IloExpr Constraint3(env); // SOMA DA DIAGONAL PRINCIPAL
	// IloExpr Constraint4(env); // SOMA DA DIAGONAL OPOSTA

	// int k = SQUARE_SIDE - 1;

	// for(int i = 0; i < SQUARE_SIDE; i++){
	// 	Constraint3 += x[i][i];
	// 	Constraint4 += x[i][k];
	// 	k--;
	// }
	// IloRange r3 = (Constraint3 == 15);
	// IloRange r4 = (Constraint4 == 15);
	// model.add(r3);
	// model.add(r4);

	
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
					std::cout << " x["<< i <<"]["<< j <<"][" << k << "] : " << cplex.getValue(x[i][j][k]);
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
	cplex.exportModel("questao1.lp");
	env.end();

	exit(0);

}