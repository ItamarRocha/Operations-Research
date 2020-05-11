#include <iostream>
#include <cmath>
#include <vector>
#include <string.h>
using namespace std;

typedef struct simplex_equation{

	vector < vector<float> > equations; //values
	vector<std::string> VB; //VB
	vector<int> constraints; // constraints

}simplex_equation;

simplex_equation simplex_constructor(vector < vector<float> > equations,vector<std::string> VB,vector<int> constraints){
	simplex_equation s1;
	s1.equations = equations;
	s1.VB = VB;
	s1.constraints = constraints;

	return s1;
}

int get_pivot_column(vector < vector <float> > equations){
	/* Getting pivot column and checking optimality */
    int pivot_column = -1;
    float min_value = 0;

    for(int j = 0; j < equations[0].size(); j++){
    	//printf("%lf\n",equations[0][j] );
    	if((equations[0][j] < 0  && equations[0][j] < min_value)){ // tem que ser < 0 e o menor valor presente
    		min_value = equations[0][j];
    		pivot_column = j;
    	}
    }
    if(pivot_column == -1){
    	printf("already optimal\n"); // se chegou -1 aqui quer dizer que já temos a solução ótima
    	return -1;
    }
	
	return pivot_column;
}

int get_pivot_row(vector < vector <float> > equations, int pivot_column){
	int pivot_row = -1;
	float minimal_ratio = 100000;
	float result;

	// dividindo a primeira pelo valor dela
	for(int i = 1; i < equations.size(); i++){ // pega do 1 porque o zero é a função que a gente quer maximizar
		
		if(equations[i][pivot_column] > 0.9){ // ta iterando pelas linhas só se ligar, porque eu acho que aqui era pra ser maior que 0
			result = equations[i][equations[0].size() - 1]/equations[i][pivot_column]; //faz o teste da razão mínima
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

void refresh_equations(vector < vector <float> > &equations,int pivot_column, int pivot_row){
	float pivot_element = equations[pivot_row][pivot_column];
	
	//divide the pivot line by the pivot element
	for(int j = 0; j < equations[0].size(); j++){
		equations[pivot_row][j] /= pivot_element;
	} // ai vai ficar o 1 direitinho
	
	//get the indices of non zero elements
	int nonzero;
	//vai zerar os outros elementos dessa coluna
	for(int i = 0; i < equations.size(); i++){

		if(equations[i][pivot_column] != 0){
			float multiplier = equations[i][pivot_column] * (-1);

			for(int j = 0; j < equations[0].size(); j++){

				if(i != pivot_row)
					equations[i][j] += equations[pivot_row][j]*multiplier; //vai adicionando o multiplicador
			}
		}
	}

}

void solve_simplex(vector < vector <float> > &equations){
	int pivot_column,pivot_row;

	while(1){
		pivot_column = get_pivot_column(equations);

		if(pivot_column == -1){
			break;
		}

		pivot_row = get_pivot_row(equations,pivot_column);
		refresh_equations(equations,pivot_column,pivot_row);
	}
}

void show_tableau(vector < vector <float> > equations){
	for (int i = 0; i < equations.size(); i++) { 
        for (int j = 0; j < equations[i].size(); j++) 
            cout << equations[i][j] << " "; 
        cout << endl; 
    } 
}

int main(int argc, char* argv[])
{
	
    // In this way we have the number of equations as row and the sum of equations and variables as the columns
    // in order to achieve a way to represente both NBV and BV(0)
    /*vector < vector<float> > equations {{0.4,0.5,0,10000,0,10000,0},
										{0.3,0.1,1,0,0,0,2.7},
										{0.5,0.5,0,1,0,0,6},
										{0.6,0.4,0,0,-1,1,6}};*/
    									

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
/*
	vector<std::string> VB {"Z","X3","X4","X5"};
	vector<int> constraints {0,0,0,0};*/

	simplex_equation s1 = simplex_constructor({{-3, -5,  0, 0, 0,  0},
    										  { 1,  0,  1, 0, 0,  4},
    										  { 0,  2,  0, 1, 0,  12},
                   							  { 3,  2,  0, 0, 1,  18} },
                   							  {"Z","X3","X4","X5"},
                   							  {0,0,0,0}
												);

/*
	cout << equations.size() << endl; // number of rows
	cout << equations[0].size() << endl; //number of columns
*/
	for(int i = 0 ; i < s1.VB.size(); i++){
		cout << s1.constraints[i] << endl;
		cout << s1.VB[i] << endl;
	}
	show_tableau(s1.equations);

    solve_simplex(s1.equations);
	
    show_tableau(s1.equations);

	return 0;
}
