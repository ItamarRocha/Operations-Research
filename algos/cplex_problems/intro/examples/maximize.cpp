#include <iostream>
#include <ilcplex/ilocplex.h>

int main(){

	IloEnv env;
	IloModel model(env);

	IloBoolVar x1(env);
	IloBoolVar x2(env);
	IloBoolVar x3(env);
	IloBoolVar x4(env);
	//adding constraints
	//model.add(IloMinimize(env,x1 + x2 + x3));
	
	model.add(2*x1 + 3*x2 + 4*x3 + 5*x4 <= 7); // creates an ilorange object
	
	//IloObjective obj = IloMinimize(env, x1 + 2*x2 + 3*x3);
	// not needed you can directly skip to adding it to the model
	model.add(IloMaximize(env,16*x1 + 19*x2 + 23*x3 + 28*x4));

	IloCplex cplex(model);

	try
	{
		cplex.solve();
	}
	catch(IloException& e)
	{
		std::cout << e;
	}
	/*
	you can check a variables value with
	IloNum Val1 = cplex.getValuue(x1);
	for example
	*/
	std::cout << "status:" << cplex.getStatus() << std::endl;
	std::cout << "Z : " << cplex.getObjValue() << std::endl;
	std::cout << "x1: " << cplex.getValue(x1) << std::endl;
	std::cout << "x2: " << cplex.getValue(x2) << std::endl;
	std::cout << "x3: " << cplex.getValue(x3) << std::endl;
	std::cout << "x4: " << cplex.getValue(x4) << std::endl;

	exit(0);
}