#include "../include/Data.hpp"
#include <stdio.h>
#include <iostream>
#include <ilcplex/ilocplex.h>

void solve(Data* d1);

int main(int argc, char* argv[]){

	if(argc != 2){
		std::cout << "Wrong Pattern\n./bpp instances/archive\n";
		exit(1);
	}
	Data d1(argv[1]);
	
	for(int i = 0; i < d1.getNEdges(); i++){
		for(int j = 0; j < d1.getNEdges(); j++){
			std::cout << d1.getVertexCapacity(i,j) << " ";
		}
		std::cout << std::endl;
	}

	//solve(&d1);

	exit(0);
}
/*
void solve(Data *d1){
	IloEnv env;
	IloModel model(env);

	IloArray < IloBoolVarArray > x(env,d1->getNItems());
	for(int i = 0; i < d1->getNItems(); i++)
    {
        IloBoolVarArray array(env, d1->getNItems());
        x[i] = array;
    }



    for(int i = 0; i <  d1->getNItems(); i++)
    {
        for(int j = 0; j < d1->getNItems(); j++)
        {
            char name[10];
            sprintf(name, "X(%d,%d)", i, j);
            x[i][j].setName(name);
            model.add(x[i][j]);
        }
    }

    IloBoolVarArray y(env,d1->getNItems());
    IloExpr FO(env);

    for(int j = 0; j < d1->getNItems(); j++){
    	FO += y[j];
    }

    model.add(IloMinimize(env,FO)); // we want to minmize it

    for(int i = 0; i < d1->getNItems(); i++){

        IloExpr Constraint1(env);
        for(int j = 0; j < d1->getNItems(); j++){
            Constraint1 += x[i][j];
        }
        IloRange r = (Constraint1 == 1);
        model.add(r);
    }

    for(int j = 0; j < d1->getNItems(); j++){

        IloExpr Constraint2(env);
        for(int i = 0; i < d1->getNItems(); i++){
            Constraint2 += d1->getItemWeight(i) * x[i][j];
        }
        IloRange r = (Constraint2  - d1->getBinCapacity() * y[j] <= 0);
        model.add(r);
    }

    IloCplex bpp(model);

    try{
        bpp.solve();
    }catch(...){
        std::cout << "deu ruim" << std::endl;
    }

	std::cout << "status:" << bpp.getStatus() << std::endl;
    std::cout << "numero de bins usados:" << bpp.getObjValue() << std::endl;

	env.end();
}*/