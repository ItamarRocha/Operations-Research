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

        double bin_capacity;
        int n_items;
        std::vector<double> weights;
        std::vector<double> values;

    public:

        Data(char* filePath);

        std::string stringRemoveBorders(std::string str);

        std::vector<std::string> stringSplit(std::string str);

        int getNItems();

        double getBinCapacity();

        double getItemWeight(int item);

        double getItemValue(int item);
};

#endif

