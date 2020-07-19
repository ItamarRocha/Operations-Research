#ifndef _DATA_H_
#define _DATA_H_

#include <vector>
#include <cstdio>
#include <cstdlib>
#include <unordered_set>
#include <iostream>

class Data
{
    public:
        int N_vertex;
        int N_edges;
        int initial_node;
        int end_node;
        std::vector<int> max_possible_flow;
        std::unordered_set<int> start_nodes;
        std::vector<std::vector<int>> capacity; //matrix of the values

        Data(char* filePath);
};

#endif

