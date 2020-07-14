#ifndef _DATA_H_
#define _DATA_H_

#include <vector>
#include <stdio.h>
#include <unordered_set>
#include <iostream>
#include <stdlib.h>

class Data
{
    private:

        int N_vertex;
        int N_edges;
        int initial_node;
        int end_node;
        std::vector<int> max_possible_flow;
        std::vector<std::vector<int>> capacity; //matrix of the values
        std::unordered_set<int> start_nodes;

    public:

        Data(char* filePath);

        void pfcm_convertion();

        int getNVertex();

        int getNEdge();

        int getEdgeCapacity(int v1, int v2);

        int getInitialNode();

        int getEndNode();

        int getMaxFlow(int i);

        std::unordered_set<int> getStartNodes();

};

#endif

