#include "../include/Data.hpp"
#include <ilcplex/ilocplex.h>
#include <chrono> 
using namespace std::chrono; 

void solve(Data* d1);

int main(int argc, char* argv[]){

	if(argc != 2){
		std::cout << "Wrong Pattern\n./pfm instances/archive\n";
		exit(1);
	}
	Data d1(argv[1]);
	d1.pfcm_convertion();

	solve(&d1);

	exit(0);
}

void solve(Data *d1){
	IloEnv env;
	IloModel model(env);
    
    int max_flow = 0;

	IloArray < IloNumVarArray > x(env,d1->getNVertex());
	for(int i = 0; i < d1->getNVertex(); i++)
    {
        IloNumVarArray array(env, d1->getNVertex(), 0, IloInfinity);
        x[i] = array;
    }

    for(int i = 0; i < d1->getNVertex(); i++)
    {   
        for(int j = 0; j < d1->getNVertex(); j++){
            char name[100];
            sprintf(name, "X(%d)", i);
            x[i][j].setName(name);
            model.add(x[i][j]);
        }
    }
    std::unordered_set<int> start_nodes = d1->getStartNodes();
    IloExpr FO(env);
    // so adiciona os que vao do inicial pro final diretamente, pois
    // equivale a um peso nessas arestas 
    for(auto start_node: start_nodes){
        //std::cout << "x[" << start_node << "][" << d1->getEndNode() <<"]" << std::endl;
    	FO += x[start_node][d1->getEndNode()];	
    }

    model.add(IloMinimize(env,FO)); // we want to minmize it


    for(int i = 0; i < d1->getNVertex(); i++){ // sum 0 constraint

        IloExpr Constraint1(env);
        IloExpr Constraint2(env);
        //std::cout << std::endl;
        for(int j = 0; j < d1->getNVertex(); j++){
            Constraint1 += x[i][j];
            Constraint1 -= x[j][i]; // sum 0 constraint

            //std::cout << "x[" << i + 1 << "][" << j + 1 <<"] = " << d1->getEdgeCapacity(i,j) << std::endl;
            Constraint2 = x[i][j] - d1->getEdgeCapacity(i,j); // capacity constraint
            IloRange r2 = (Constraint2 <= 0); // nunca pode exceder a capacidade
            model.add(r2);

            //std::cout << "+ x[" << i + 1 << "][" << j + 1 <<"] - x["<< j + 1<< "][" << i + 1<< "] "; 

        }
        if(start_nodes.find(i) != start_nodes.end()){
        	//std::cout <<"maxflow "<< d1->getMaxFlow(i) << std::endl;
            //1std::cout << " = " << d1->getMaxFlow(i) << std::endl;
            IloRange r = (Constraint1 == d1->getMaxFlow(i)); // limits the flow on each initial node
            model.add(r);
        }else if( i == d1->getEndNode()){

        	for(auto start_node: start_nodes){
	           	max_flow += d1->getMaxFlow(start_node); //limits to the maximum flow in the exit node which is the sum of the flows 
            }
            //std::cout << " = " << max_flow * -1 << std::endl; 
            IloRange r = (Constraint1 == max_flow* -1);
        	model.add(r);
    	
        }else{
            //std::cout << " = 0" << std::endl;
            IloRange r = (Constraint1 == 0);
            model.add(r); //any transhipment node must sum 0
        }
    }


    IloCplex mfp(model);

	auto start = high_resolution_clock::now(); 

    try{
        mfp.solve();
    }catch(...){
        std::cout << "deu ruim" << std::endl;
    }
    auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start); 
	  
	std::cout << "duration = " << duration.count() << std::endl; 

	std::cout << "status:" << mfp.getStatus() << std::endl;
    std::cout << "Objective function:" << mfp.getObjValue() << std::endl;
    for(int i = 0; i < d1->getNVertex(); i++){
        for(int j = 0; j < d1->getNVertex(); j++){
        	if(mfp.getValue(x[i][j]) > 0.01)
            	std::cout << " X[" << i + 1 << "][" << j + 1 << "] = " << mfp.getValue(x[i][j]);
        }
        std::cout << std::endl;
    }
    // o fluxo maximo vai ser o maximo possivel menos o que passou pelo arco que tem custo
    std::cout << "Max flow = " << max_flow - mfp.getObjValue() << std::endl; //remove the number of paths that went through the penalty paths


    mfp.exportModel("model.lp");

	env.end();
}