#include <iostream>
#include <ilcplex/ilocplex.h>
#define SIZE 10
#define SQUARE_SIDE 3
int main(){

	IloEnv env;
	IloModel model(env);


	IloArray < IloNumVarArray > x(env,SQUARE_SIDE);
	// criando um array pras variaveis e adicionando as ao modelo
	for(int i = 0; i < SQUARE_SIDE; i++){
		IloNumVarArray array(env, SQUARE_SIDE, 0, 9);
		x[i] = array;
	}

	for(int i = 0; i < SQUARE_SIDE; i++){
		for(int j = 0; j < SQUARE_SIDE; j++){
			char name[SIZE];
			sprintf(name,"X%d%d",i,j);
			x[i][j].setName(name);
			model.add(x[i][j]);
		}
	}


	IloExpr FO(env);
	int n = 1;
	for(int i = 0; i < SQUARE_SIDE; i++){
		for(int j = 0; j < SQUARE_SIDE; j++){
			FO += n - x[i][j];
			n++;
		}
	}
	model.add(IloMinimize(env,FO));

	// could have done all restriction in only one double loop but
	// for some reason it was giving me segmentation fault. (ill try to solve
	// this later ).
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
	IloExpr Constraint3(env); // SOMA DA DIAGONAL PRINCIPAL
	IloExpr Constraint4(env); // SOMA DA DIAGONAL OPOSTA

	int k = SQUARE_SIDE - 1;

	for(int i = 0; i < SQUARE_SIDE; i++){
		Constraint3 += x[i][i];
		Constraint4 += x[i][k];
		k--;
	}
	IloRange r3 = (Constraint3 == 15);
	IloRange r4 = (Constraint4 == 15);
	model.add(r3);
	model.add(r4);

	
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
			std::cout << " x["<< i <<"]["<< j <<"] : " << cplex.getValue(x[i][j]);
		}
		std::cout << std::endl;
	}
	cplex.exportModel("questao1.lp");
	env.end();

	exit(0);

}