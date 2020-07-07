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
    public:

        Data(char* fileMatrix, char* fileWeights, char* fileRows, char* fileColumns);
        std::string stringRemoveBorders(std::string str);
        std::vector<std::string> stringSplit(std::string str);

        int getRowsNumber();
        int getColumnsNumber();

};

#endif

