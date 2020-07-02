#include "../include/Data.hpp"
#include <stdlib.h>

Data::Data(char* filePath)
{
    FILE* f = fopen(filePath, "r");

    if(!f)
    {
        printf("Problem while reading instance. Check file.\n");
        exit(1);
    }

    if(fscanf(f, "%d", &N_edges) != 1)
    {
        printf("Problem while reading instance. Check Edges.\n");
        exit(1);
    }

    if(fscanf(f, "%d", &N_vertex) != 1)
    {
        printf("Problem while reading instance. Check Vertex.\n");
        exit(1);
    }

    if(fscanf(f, "%d", &initial_node) != 1)
    {
        printf("Problem while reading instance. Check Initial node.\n");
        exit(1);
    }

    if(fscanf(f, "%d", &end_node) != 1)
    {
        printf("Problem while reading instance. Check End node.\n");
        exit(1);
    }

    int edge1, edge2, cap;
    //reading capacity(A_NUMBER,std::vector<int>(OTHER_NUMBER, 4));
    capacity = std::vector<std::vector <int>> (N_edges, std::vector<int>(N_edges));
    for(int i = 0; i < N_vertex; i++) // iterates through the vertex, to store the capacity since it 
        // is given in function of the edges
    {

        if(fscanf(f, "%d %d %d", &edge1, &edge2, &cap) != 3) //returns the number of sucessfully assigned input items
        {
            printf("%d\n",cap);
            printf("Problem while reading instance.\n");
            exit(1);
        }
        capacity[edge1][edge2] = cap;
    }

    fclose(f);
}

int Data::getNEdges()
{
    return N_edges;
}

int Data::getNVertex()
{
    return N_vertex;
}

int Data::getVertexCapacity(int e1, int e2)
{
    return capacity[e1][e2]; 
}
