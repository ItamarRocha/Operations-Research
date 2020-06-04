#ifndef _DATA_H_
#define _DATA_H_

#include <vector>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>

class Data
{
    private:

        int bin_capacity;
        int n_items;
        std::vector<double> weights;
        std::vector<double> values;

    public:

        Data(char* filePath);

        std::string stringRemoveBorders(std::string str);

        std::vector<std::string> stringSplit(std::string str);

        int getNItems();

        int getBinCapacity();

        int getItemWeight(int item);

        int getItemValue(int item);
};

#endif

