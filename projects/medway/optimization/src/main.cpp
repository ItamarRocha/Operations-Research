#include "../include/Data.hpp"
#include <ilcplex/ilocplex.h>
#define MAX_SIZE 1000

void solve(Data* d1);

int main(int argc, char* argv[]){

	if(argc != 5){
		std::cout << "Wrong Pattern\n./bpp instances/compatibility_matrix instances/weights instances/rows instances/columns\n";
		exit(1);
	}
	Data d1(argv[1], argv[2], argv[3], argv[4]);
	solve(&d1);

	exit(0);
}

void solve(Data *d1){
	IloEnv env;
	IloModel model(env);

	// 	
	// DECISION VARIABLE -> In this project, the decision variable consists in a boolean array with rows representing students 
	// and columns representing advisors. Each array variable represents if a student is placed with the correspondent advisor
	//

	IloArray < IloBoolVarArray > x(env, d1->getNRows());
	for(int i = 0; i < d1->getNRows(); i++)
    {
        IloBoolVarArray array(env, d1->getNColumns());
        x[i] = array;
    }

	// 
	// OBJECTIVE FUNCTION -> In this project, our objective function will be to maximize the
	// the sum of compatibilities(total) between students and advisors; 
    //

    IloExpr FO(env);

    for(int i = 0; i <  d1->getNRows(); i++)
    {
        for(int j = 0; j < d1->getNColumns(); j++)
        {
            std::ostringstream name;
            name << "X[" << d1->getRowName(i) << "][" << d1->getColumnName(j) << "]";
            std::cout << name.str().c_str() << std::endl;
            x[i][j].setName(name.str().c_str());
            model.add(x[i][j]);

            FO += x[i][j] * d1->getCompatibility(i,j);
        }
    }

    model.add(IloMaximize(env,FO)); // we want to maximize it as said before

	// 
	// Constraint1 -> Limiting each student to have only one advisor
	//     

    for(int i = 0; i < d1->getNRows(); i++){

        IloExpr Constraint1(env);
        for(int j = 0; j < d1->getNColumns(); j++){
            Constraint1 += x[i][j];
        }
        IloRange r = (Constraint1 == 1);
	    model.add(r);
	}

	// 
	// Constraint2 -> Limiting the number of students per advisor
	// 

    for(int j = 0; j < d1->getNColumns(); j++){

        IloExpr Constraint2(env);
        for(int i = 0; i < d1->getNRows(); i++){
            Constraint2 += x[i][j];
        }
        IloRange r = (Constraint2  -  11 <= 0);
        model.add(r);
    }

    IloCplex medway(model);

    try{
        medway.solve();
    }catch(...){
        std::cout << "deu ruim" << std::endl;
    }

	std::cout << "status:" << medway.getStatus() << std::endl;
    std::cout << "Total Compat:" << medway.getObjValue() << std::endl;

    for(int i = 0; i < d1->getNColumns(); i++){
    	for(int j = 0; j < d1->getNRows(); j++){
    		int value = medway.getValue(x[j][i]);
    		if(value > 0)
    			std::cout << x[j][i].getName() << std::endl;
    	}
    }
	env.end();
}