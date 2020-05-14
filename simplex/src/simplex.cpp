#include <iostream>
#include <cmath>
#include <vector>
#include <string.h>
#include "parser.h"
using namespace std;

typedef struct simplex_equation{

	vector < vector<double> > equations; //values
	vector<int> VB; //VB o 0 é a equação
	//vector<int> constraints; // constraints
	vector<int> M_preprocess_erase;

}simplex_equation;

simplex_equation simplex_constructor(vector < vector<double> > equations,vector<int> VB){//,vector<int> constraints){
	
	simplex_equation s1;
	s1.equations = equations;
	s1.VB = VB;
	//s1.constraints = constraints;

	for (int j = 0; j < s1.equations[0].size(); j++) { 
        if (equations[0][j] > 9000)
        {	
        	s1.M_preprocess_erase.push_back(j);
        }
    }

	return s1;
}

void preprocess_equations(simplex_equation* s1){

	int row;
	double multiplier;
	for(int k = 0; k < s1->M_preprocess_erase.size(); k++){
		//cout << s1->M_preprocess_erase[k];
		row = -1;
		multiplier = -1;

		for(int i = 0; i < s1->equations.size(); i++){
			if(s1->equations[i][s1->M_preprocess_erase[k]] > 0)
				row = i; // pega a fileira que a gente tem que multiplicar pra zerar o M grande
		}

		multiplier = s1->equations[0][s1->M_preprocess_erase[k]] * (-1);

		for(int j = 0; j < s1->equations[0].size(); j++){
			s1->equations[0][j] += s1->equations[row][j] * multiplier;
		}
	}

	//cout << "row = " << row << endl;
}

int get_pivot_column(simplex_equation s1){
	/* Getting pivot column and checking optimality */
    int pivot_column = -1;
    double min_value = 0;

    //cout << "opa" << s1.constraints[0] << endl;

    for(int j = 0; j < s1.equations[0].size() - 1; j++){ // -1 pra excluir a ultima coluna que é a das soluções
    	//printf("%lf\n",equations[0][j] );
    	if((s1.equations[0][j] < 0  && s1.equations[0][j] < min_value)){ // tem que ser < 0 e o menor valor presente
    		min_value = s1.equations[0][j];
    		pivot_column = j;
    	}
    }
    if(pivot_column == -1){
    	printf("already optimal\n"); // se chegou -1 aqui quer dizer que já temos a solução ótima
    	return -1;
    }
	
	return pivot_column;
}

int get_pivot_row(simplex_equation s1, int pivot_column){
	int pivot_row = -1;
	double minimal_ratio = 100000;
	double result;

	// dividindo a primeira pelo valor dela
	for(int i = 1; i < s1.equations.size(); i++){ // pega do 1 porque o zero é a função que a gente quer maximizar
		
		if(s1.equations[i][pivot_column] > 0){ // ta iterando pelas linhas só se ligar, porque eu acho que aqui era pra ser maior que 0
			result = s1.equations[i][s1.equations[0].size() - 1]/s1.equations[i][pivot_column]; //faz o teste da razão mínima
			// basicament divide os valores das VB pelos valores da coluna pivot. o Menor valor
			// razao minima vai ser usado pra dizer a linha pivot
			if(result < minimal_ratio){
				minimal_ratio = result;
				pivot_row = i;
			}
		}
	}

	return pivot_row; 
}

void refresh_equations(simplex_equation * s1,int pivot_column, int pivot_row){

	double pivot_element = s1->equations[pivot_row][pivot_column];
	
	s1->VB[pivot_row] = pivot_column + 1;

	//divide the pivot line by the pivot element
	for(int j = 0; j < s1->equations[0].size(); j++){
		s1->equations[pivot_row][j] /= pivot_element;
	} // ai vai ficar o 1 direitinho
	
	//get the indices of non zero elements
	int nonzero;
	//vai zerar os outros elementos dessa coluna
	for(int i = 0; i < s1->equations.size(); i++){

		if(s1->equations[i][pivot_column] != 0){
			double multiplier = s1->equations[i][pivot_column] * (-1);

			for(int j = 0; j < s1->equations[0].size(); j++){

				if(i != pivot_row)
					s1->equations[i][j] += s1->equations[pivot_row][j]*multiplier; //vai adicionando o multiplicador
			}
		}
	}

}

void solve_simplex(simplex_equation* s1){

	int pivot_column,pivot_row;

	while(1){
		pivot_column = get_pivot_column(*s1);

		if(pivot_column == -1){
			break;
		}

		pivot_row = get_pivot_row(*s1,pivot_column);
		refresh_equations(s1,pivot_column,pivot_row);
	}
}

/*void show_tableau(vector < vector<double> > equations){
	for (int i = 0; i < equations.size(); i++) { 
        for (int j = 0; j < equations[i].size(); j++) 
            cout << equations[i][j] << " "; 
        cout << endl; 
    } 
}*/

int main(int argc, char* argv[])
{

	if(argc < 2){
		std::cout << "Wrong Pattern\n" << "EX: \n" << "./simplex [filename]" << std::endl;
		exit(-1);
	}

	std::string filename = argv[1];
	//strcpy(filename,argv[1]);
	
    // In this way we have the number of equations as row and the sum of equations and variables as the columns
    // in order to achieve a way to represente both NBV and BV(0)
    /*vector < vector<float> > equations {{0.4,0.5,0,10000,0,10000,0},
										{0.3,0.1,1,0,0,0,2.7},
										{0.5,0.5,0,1,0,0,6},
										{0.6,0.4,0,0,-1,1,6}};*/ //Z = -5.25 X1 = 7.5 X5 = 0.3 X2 = 4.5

    									

    								    	/*{{-3, -5,  0, 0, 0,  0}, // ou 0
    										{ 1,  0,  1, 0, 0,  4},
    										{ 0,  2,  0, 1, 0,  12},
                   							{ 3,  2,  0, 0, 1,  18} }; //optimal = 36 ->2 6 2
                   					*/
                   					/*{{-1,-1,-1,0,0,0},
    									 {1,1,0,1,0,1 },
    									 {0,-1,1,0,1,0}};*/
	
                   					/* { 	{-40, -50,  0, 0, 0,  0},
    										{ 1,  2,  1, 0, 0,  12},
    										{ 5,  4,  0, 1, 0,  30},
                   							{ 3,  1,  0, 0, 1,  15} };*/
									/*	{{-2,-1,-2,0,0,0,0},
										 {2,1,0,1,0,0,10},
										 {1,2,-2,0,1,0,20},
										 {0,1,2,0,0,1,5}} //Z = 15 X1 = 5 X5 = 20 X3 = 2.5

/*
	vector<std::string> VB {"Z","X3","X4","X5"};
	vector<int> constraints {0,0,0,0};*/

/*	simplex_equation s1 = simplex_constructor({{0.4,0.5,0,10000,0,10000,0},
										{0.3,0.1,1,0,0,0,2.7},
										{0.5,0.5,0,1,0,0,6},
										{0.6,0.4,0,0,-1,1,6}},
                   							  {0,4,5,6} //no primeiro refere-se a FO // 0 max // 1 min
                   							  			//o resto é cada constraint // 0 <= // 1 = // 2 >=  
												);
*/
/*
	cout << equations.size() << endl; // number of rows
	cout << equations[0].size() << endl; //number of columns
*/	

	vector < vector<double> > v1 = get_tableau(filename);

	simplex_equation s1 = simplex_constructor(v1,
                   							  {0,4,5} //no primeiro refere-se a FO // 0 max // 1 min
                   							  			//o resto é cada constraint // 0 <= // 1 = // 2 >=  
												);

	preprocess_equations(&s1);

	//show_tableau(s1.equations);

    solve_simplex(&s1);
	
    show_tableau(s1.equations);

    for(int i = 0; i < s1.VB.size(); i++){
    	if (i == 0)
    	{
   			cout << "Z" << " = " << s1.equations[i][s1.equations[0].size()-1]<<endl;
       	}else{
    		cout << "X" <<s1.VB[i]<< " = " << s1.equations[i][s1.equations[0].size()-1]<<endl;
    
    	}
    }

	
	//show_tableau(v1);


	return 0;
}
