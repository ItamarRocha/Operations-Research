#include <iostream>
#include <ilcplex/ilocplex.h>

int main(){

	IloEnv env;
	IloModel model(env);

	std::cout << "opa" << std::endl;
	IloNumVar x1(env,ILOFLOAT);
	IloNumVar x2(env,ILOFLOAT);
	//adding constraints
	//model.add(IloMinimize(env,x1 + x2 + x3));
	
	model.add(2*x1 + x2 <= 10); // creates an ilorange object
	model.add(3*x1 + 3*x2 <= 20); // creates an ilorange object
	model.add(2*x1 + 4*x2 <= 20);
	std::cout << "opa" << std::endl;

	//IloObjective obj = IloMinimize(env, x1 + 2*x2 + 3*x3);
	// not needed you can directly skip to adding it to the model
	model.add(IloMaximize(env,20*x1 + 30*x2));
	std::cout << "opa" << std::endl;
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
	std::cout << "x1 : " << cplex.getValue(x1) << std::endl;
	std::cout << "x2 : " << cplex.getValue(x2) << std::endl;

	exit(0);
}