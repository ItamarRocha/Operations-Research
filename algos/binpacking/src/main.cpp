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
	solve(&d1);

	exit(0);
}

void solve(Data *d1){
	IloEnv env;
	IloModel model(env);

	/*
		-------- Decision Variable ----------
		Creates the boolean 2D array respnsible to store where
		each item is stored
	*/

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
    /*
		----------- FO ------------
		creates de array representing the activation of the bins
		and adds it sum to the Objective Function in order to mi
		nimize the number of bins
    */

    IloBoolVarArray y(env,d1->getNItems());
    IloExpr FO(env);

    for(int j = 0; j < d1->getNItems(); j++){
    	FO += y[j];
    }

    model.add(IloMinimize(env,FO)); // we want to minmize it

    /*
		---------- Constraints -----------
		1. each item must be only in one bin
		2. The sum of the weight of the items
		must be less or equal to its capacity
    */
    for(int i = 0; i < d1->getNItems(); i++){
        /*
            First we iterate through each item, summing its values
            of xij, meaning that it can only be present at one j
            when we finish row, we go to the next one and redefine
            the constraint1 variable in order to add the next item
            constraint and so on till we finish the table
        */
        IloExpr Constraint1(env);
        for(int j = 0; j < d1->getNItems(); j++){
            Constraint1 += x[i][j];
        }
        IloRange r = (Constraint1 == 1);
        model.add(r);
    }

    for(int j = 0; j < d1->getNItems(); j++){
        /*
            we loop through the bins and the through the items. The
            sum of the items weight must be less than the bins capa
            city, so after we add this constraint we go to the next
            bin and redeclare the constraint variable.
        */
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
    /*for(int i = 0; i < d1->getNItems(); i++) 
    {
        for(int k = 0; k < d1->getNItems(); k++)
        {
            if(bpp.getValue(x[i][k]) > 0.9)
            {
                std::cout << "item " << i << " no bin " << k << std::endl;
            }
        }
    }*/

	env.end();
}