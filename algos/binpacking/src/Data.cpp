#include "../include/Data.hpp"
#include <stdlib.h>

Data::Data(char* filePath)
{
    FILE* f = fopen(filePath, "r");

    if(!f)
    {
        printf("Problem while reading instance.\n");
        exit(1);
    }

    if(fscanf(f, "%d", &n_items) != 1)
    {
        printf("Problem while reading instance.\n");
        exit(1);
    }

    if(fscanf(f, "%d", &bin_capacity) != 1)
    {
        printf("Problem while reading instance.\n");
        exit(1);
    }

    //reading weights
    weights = std::vector<int>(n_items, 0);
    for(int i = 0; i < n_items; i++)
    {
        if(fscanf(f, "%d", &weights[i]) != 1)
        {
            printf("Problem while reading instance.\n");
            exit(1);
        }
    }

    fclose(f);
}

int Data::getNItems()
{
    return n_items;
}

int Data::getBinCapacity()
{
    return bin_capacity;
}

int Data::getItemWeight(int item)
{
    return weights[item]; 
}
