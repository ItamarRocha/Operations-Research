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
		X[i] = array;
	}

	for(int i = 0; i < SQUARE_SIDE; i++){
		for(int j = 0; j < SQUARE_SIDE; j++){
			char name[SIZE];
			sprintf(name,"X[%d][%d]",i+1,j+1);
			x[i][j].setname(name)
			model.add(x[i][j]);
		}
	}

	int k = SQUARE_SIDE - 1;

	IloExpr Constraint3(env); // SOMA DA DIAGONAL PRINCIPAL
	IloExpr Constraint4(env); // SOMA DA DIAGONAL OPOSTA
		
	for(int i = 0; i < SQUARE_SIDE; i++){

		IloExpr Constraint1(env); // PEGA A SOMA DAS LINHAS
		IloExpr Constraint2(env); // PEGA A SOMA DAS COLUNAS
		
		for(int j = 0; i < SQUARE_SIDE; i++){
			Constraint1 += x[i][j];		
			Constraint2 += x[j][i];

		}
		model.add(Constraint1 <= 15);
		model.add(Constraint2 <= 15);

		Constraint3 += x[i][i];
		Constraint4 += x[i][k];

		k--;
	}
	model.add(Constraint3 <= 15);
	model.add(Constraint4 <= 15);

	IloExpr FO(env);

	for(int i = 0; )


	IloCplex cplex(model);

	/*try
	{
		cplex.solve();
	}
	catch(IloException& e)
	{
		std::cout << e;
	}

	std::cout << "status:" << cplex.getStatus() << std::endl;
	std::cout << "Z : " << cplex.getObjValue() << std::endl;
	std::cout << "x1 : " << cplex.getValue(x1) << std::endl;
	std::cout << "x2 : " << cplex.getValue(x2) << std::endl;
	std::cout << "x3 : " << cplex.getValue(x3) << std::endl;
	std::cout << "x4 : " << cplex.getValue(x4) << std::endl;
	std::cout << "x5 : " << cplex.getValue(x5) << std::endl;

	cplex.exportModel("questao1.lp");
*/
	env.end();

	exit(0);

}