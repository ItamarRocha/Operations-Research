#include "../include/Data.hpp"
#include <ilcplex/ilocplex.h>

void solve(Data* d1);

int main(int argc, char* argv[]){
	
	if(argc < 2){
		std::cout << "Wrong Pattern, Type the path to the instance" << std::endl;
	}

	Data d1 = Data(argv[1]);

	solve(&d1);

	return 0;

}

void solve(Data* d1){
	IloEnv env;
	IloModel model(env);

	IloBoolVarArray x(env, d1->getNItems()); // xi decision variables
	
	for(int i = 0; i < d1->getNItems(); i++){
		char name[20];
		sprintf(name,"X(%d)",i); //naming them
		x[i].setName(name);
		model.add(x[i]); // adding the decisions to the model
	}

	IloExpr FO(env); // creating the objective function

	for(int i = 0; i < d1->getNItems(); i++){
		FO += d1->getItemValue(i) * x[i]; // adding the sum of values * xi 
	}

	model.add(IloMaximize(env,FO)); //the objective is to maximize the total value of the sack

	IloExpr constraints(env); //defining the constraints
	for(int i = 0; i < d1->getNItems(); i++){
		constraints += d1->getItemWeight(i) * x[i]; // weight * xi
	}
	IloRange r = (constraints <= d1->getBinCapacity());
	/*
		The constraint in this problem is that the total weight should not be bigger then the
		sack capacity
	*/
	model.add(r);

	IloCplex knapsack(model);

	try{
		knapsack.solve();
	}
	catch(IloException& e){
		std::cout << "error" << std::endl;
	}

	std::cout << "status:" << knapsack.getStatus() << std::endl;
    std::cout << "Valor total : " << knapsack.getObjValue() << std::endl;

    /*for(int i = 0; i < d1->getNItems(); i++){
    	if(knapsack.getValue(x[i]))
    		std::cout << "item " << i << " selected" << std::endl;

    }*/
    
    env.end();

    exit(0);
}
