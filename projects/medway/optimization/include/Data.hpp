#ifndef _DATA_H_
#define _DATA_H_

#include <vector>
#include <stdio.h>

class Data
{
    private:

        int bin_capacity;
        int n_items;
        std::vector<int> weights;

    public:

        Data(char* filePath);

        int getNItems();

        int getBinCapacity();

        int getItemWeight(int item);
};

#endif

