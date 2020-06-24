#include <iostream>
#include <ilcplex/ilocplex.h>

int main(){

	IloEnv env;
	IloModel model(env);

	std::cout << "opa" << std::endl;
	IloNumVar x1(env, 0, IloInfinity, ILOFLOAT);
	IloNumVar x2(env, 0, IloInfinity, ILOFLOAT);
	IloNumVar x3(env, 0, IloInfinity, ILOFLOAT);
	IloNumVar x4(env, 0, IloInfinity, ILOFLOAT);
	IloNumVar x5(env, 0, IloInfinity, ILOFLOAT);

	model.add(-0.2*x1 + 0.05*x2 -0.3*x3 + 0.7*x4 - 0.3*x5 >= 0); // creates an ilorange object
	model.add(0.035*x1 - 0.005*x2 - 0.01*x3 - 0.015*x4 - 0.015*x5 >= 0); // creates an ilorange object
	model.add(0.005*x1 - 0.025*x2 + 0.045*x3 - 0.035*x4 + 0.965*x5 >= 0);

	model.add(x1 + x2 + x3 + x4 + x5 == 1); // porque é uma tonelada o necessário

	model.add(IloMinimize(env,1200*x1 + 1800*x2 + 500*x3 + 10000*x4 + 3800*x5));
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

	std::cout << "status:" << cplex.getStatus() << std::endl;
	std::cout << "Z : " << cplex.getObjValue() << std::endl;
	std::cout << "x1 : " << cplex.getValue(x1) << std::endl;
	std::cout << "x2 : " << cplex.getValue(x2) << std::endl;
	std::cout << "x3 : " << cplex.getValue(x3) << std::endl;
	std::cout << "x4 : " << cplex.getValue(x4) << std::endl;
	std::cout << "x5 : " << cplex.getValue(x5) << std::endl;

	exit(0);
}