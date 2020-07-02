#include "../include/Data.hpp"
#include <stdio.h>
#include <iostream>
#include <ilcplex/ilocplex.h>
#define M 10000

void solve(Data* d1);

int main(int argc, char* argv[]){

	if(argc != 2){
		std::cout << "Wrong Pattern\n./bpp instances/archive\n";
		exit(1);
	}
	Data d1(argv[1]);
	d1.pfcm_convertion();
	for(int i = 0; i < d1.getNEdges(); i++){
		for(int j = 0; j < d1.getNEdges(); j++){
			std::cout << d1.getVertexCapacity(i,j) << " ";
		}
		std::cout << std::endl;
	}

	solve(&d1);

	exit(0);
}

void solve(Data *d1){
	IloEnv env;
	IloModel model(env);


    // Initializing the array x_ij that represents the flow
	IloArray < IloNumVarArray > x(env,d1->getNEdges());
	for(int i = 0; i < d1->getNEdges(); i++)
    {
        IloNumVarArray array(env, d1->getNEdges(), 0, IloInfinity);
        x[i] = array;
    }

    for(int i = 0; i < d1->getNEdges(); i++)
    {   
        for(int j = 0; j < d1->getNEdges(); j++){
            char name[100];
            sprintf(name, "X(%d)", i);
            x[i][j].setName(name);
            model.add(x[i][j]);
        }
    }

    IloExpr FO(env);

    FO += M * x[d1->getInitialNode()][d1->getEndNode()];

    model.add(IloMinimize(env,FO)); // we want to minmize it


    for(int i = 0; i < d1->getNEdges(); i++){

        IloExpr Constraint1(env);

        for(int j = 0; j < d1->getNEdges(); j++){
            Constraint1 += x[i][j];
            Constraint1 -= x[j][i];
        }
        if( i == d1->getInitialNode() ){
            IloRange r = (Constraint1 == d1->getMaxFlow());
            model.add(r);
        }else if( i == d1->getEndNode()){
            IloRange r = (Constraint1 == d1->getMaxFlow()* -1);
            model.add(r);
        }else{
            IloRange r = (Constraint1 == 0);
            model.add(r);
        }
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
}