#ifndef _DATA_H_
#define _DATA_H_

#include <vector>
#include <stdio.h>

class Data
{
    private:

        int N_edges;
        int N_vertex;
        int initial_node;
        int end_node;
        std::vector<std::vector<int>> capacity; //matrix of the values

    public:

        Data(char* filePath);

        int getNEdges();

        int getNVertex();

        int getVertexCapacity(int e1, int e2);
};

#endif

