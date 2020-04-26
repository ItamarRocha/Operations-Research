#include <iostream>
#include <cmath>
#include <vector>
#include <string.h>
using namespace std;

class simplex_solver{
	public:
		int number_of_variables;
		int number_of_equations;
		float oip;


};

int get_pivot_column(vector < vector <float> > equations){
	/* Getting pivot column and checking optimality */
    int pivot_column = -1;
    float value = 0;
    for(int j = 0; j < equations[0].size(); j++){
    	if(equations[0][j] < 0 && equations[0][j] < value){
    		value = equations[0][j];
    		pivot_column = j;
    	}  
    }
    if(pivot_column == -1){
    	printf("already optimal\n");
    	return 2;
    }
	
	return pivot_column;
}

int get_pivot_row(vector < vector <float> > equations, int pivot_column){
	int pivot_row = -1;
	float minimal_ratio = 100000;
	float result;

	// dividindo a primeira pelo valor dela
	for(int i = 1; i < equations.size(); i++){
		if(equations[i][pivot_column] > 0.9){
			result = equations[i][equations[0].size() - 1]/equations[i][pivot_column];
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
	}
	
	//get the indicies of non zero elements
	int nonzero;
	for(int i = 0; i < equations.size(); i++){
		if(equations[i][pivot_column] != 0){
			float multiplier = equations[i][pivot_column] * (-1);
			for(int j = 0; j < equations[0].size(); j++){
				if(i != pivot_row)
					equations[i][j] += equations[pivot_row][j]*multiplier;
			}
		}
	}

}


int main(int argc, char* argv[])
{
	/*if(argc > 1 && !strcmp(argv[1],"-r"))// flag to do a test with the variables inside the code or to scan the variables with scanf
		printf("Type the objective function parameters\n");
		int a,b,c,d,e;
		cin >> a >> b >> c >> d >> e;
		int C[] = {a,b,c,d,e};
		printf("%d\n", C[3]);
		return 0;
*/	
	
    // In this way we have the number of equations as row and the sum of equations and variables as the columns
    // in order to achieve a way to represente both NBV and BV
    vector < vector<float> > equations 	 { 	{-40, -50,  0, 0, 0,  0},
    										{ 1,  2,  1, 0, 0,  12},
    										{ 5,  4,  0, 1, 0,  30},
                   							{ 3,  1,  0, 0, 1,  15} };
                   					/*{ 	{-3, -5,  0, 0, 0,  0},
    										{ 1,  0,  1, 0, 0,  4},
    										{ 0,  2,  0, 1, 0,  12},
                   							{ 3,  2,  0, 0, 1,  18} };*/

	cout << equations.size() << endl;
	cout << equations[0].size() << endl;

	for (int i = 0; i < equations.size(); i++) { 
        for (int j = 0; j < equations[i].size(); j++) 
            cout << equations[i][j] << " "; 
        cout << endl; 
    } 


    
    int pivot_column = get_pivot_column(equations);
	cout <<"Pivot : " << pivot_column << endl;	


	int pivot_row = get_pivot_row(equations, pivot_column);
	cout << "pivot_row = " << pivot_row << endl;

	refresh_equations(equations,pivot_column, pivot_row);

	for (int i = 0; i < equations.size(); i++) { 
        for (int j = 0; j < equations[i].size(); j++) 
            cout << equations[i][j] << " "; 
        cout << endl; 
    } 

    pivot_column = get_pivot_column(equations);
	cout <<"Pivot : " << pivot_column << endl;	


	pivot_row = get_pivot_row(equations, pivot_column);
	cout << "pivot_row = " << pivot_row << endl;

	refresh_equations(equations,pivot_column, pivot_row);

	for (int i = 0; i < equations.size(); i++) { 
        for (int j = 0; j < equations[i].size(); j++) 
            cout << equations[i][j] << " "; 
        cout << endl; 
    } 

	return 0;
}
