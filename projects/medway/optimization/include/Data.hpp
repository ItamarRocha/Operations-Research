#ifndef _DATA_H_
#define _DATA_H_

#include <vector>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
class Data
{
    private:
    	std::vector<std::vector<int>> compatibility_matrix;
    	std::vector<int> time_weights;
    	std::vector<std::string> rows;
    	std::vector<std::string> columns;
    	int rows_number; //to get the size of the rows in order to dimension the compatibility and the weights vectors
    	int columns_number; // to get the columns size
        std::vector<int> NFixedStudents; // vector to store the 
        std::vector<int> LowerBoundTime;

    public:

        Data(char* fileMatrix, char* fileWeights, char* fileRows, char* fileColumns, char* lb);
        std::string stringRemoveBorders(std::string str);
        std::vector<std::string> stringSplit(std::string str);

        int getNRows();
        int getNColumns();

        std::string getRowName(int index);
        std::string getColumnName(int index);

        int getCompatibility(int i, int j);
        int getTimeWeight(int i);

        int getLowerBoundTime(int assessor_index);
        int getNFixedStudents(int assessor_index);
};

#endif

