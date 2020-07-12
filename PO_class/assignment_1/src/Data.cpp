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

    if(fscanf(f, "%d", &N_vertex) != 1)
    {
        printf("Problem while reading instance. Check Edges.\n");
        exit(1);
    }

    if(fscanf(f, "%d", &N_edges) != 1)
    {
        printf("Problem while reading instance. Check Archs.\n");
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

    int vertex1, vertex2, cap;
    //reading capacity(A_NUMBER,std::vector<int>(OTHER_NUMBER, 4));
    capacity = std::vector<std::vector <int>> (N_vertex, std::vector<int>(N_vertex));

    std::unordered_set <int> vertices2; //usa unordered set pq a consulta é O(1)
    std::unordered_set <int> vertices1; //se usasse vector aqui quando fosse checar com .find ia ser O(n)

    for(int i = 0; i < N_edges; i++) // iterates through the vertex, to store the capacity since it 
        // is given in function of the edges
    {
        if(fscanf(f, "%d %d %d", &vertex1, &vertex2, &cap) != 3) //returns the number of sucessfully assigned input items
        {
            printf("%d\n",cap);
            printf("Problem while reading instance.\n");
            exit(1);
        }
        capacity[vertex1][vertex2] = cap;
        // if(vertex1 == initial_node){
        //     max_possible_flow += cap;
        // }
        if(vertices1.find(vertex1) == vertices1.end()){ // checa se ja ta inserido pra nao inserir dnv
            vertices1.insert(vertex1);
        }   

        if(vertices2.find(vertex2) == vertices2.end()){
            vertices2.insert(vertex2);
        }

    }

    for(auto const element: vertices2){ // pega só os nós que são origens
        if(vertices1.find(element) != vertices1.end()) // se tiver algum dos vertices2 ( nos de destino ) ele apaga
            vertices1.erase(element);
    }

    start_nodes = vertices1; //só ficou os nós de início (ou seja, os que nao tem ngn enviando pra eles)

    max_possible_flow = std::vector <int> (start_nodes.size());
    //std::cout << "start nodes size "<< start_nodes.size() << std::endl;

    // 
    // Aqui a gente ta iterando por cada no de entrada e somando a capacidade maxima de saida
    // de cada um desses nós para cada no transshipment e somando pra ver qual a capacidade
    // máxima do nó de entrada.
    //
    for (int i = 0; i < start_nodes.size(); i++)
    {
        for(int j = 0; j < N_vertex; j++){
            max_possible_flow[i] += capacity[i][j];
            // std::cout << capacity[i][j] << std::endl;
            // std::cout <<"max flow ["<< i << "]" << max_possible_flow[i] << std::endl;
       }
    }

    fclose(f);
}

void Data::pfcm_convertion(){
    // aqui a gente adiciona um caminho indo dos nós de entrada pro nó de saída.
    // como colocado na modelagem, só vai passar por aqui oq nao passar pela network
    // normal, já que o custo pra passar por ela é zero e o custo pra passar por aqui
    // vai ser maior que zero
    for(auto start_node : start_nodes){
        capacity[start_node][end_node] = max_possible_flow[start_node];
    }
}

int Data::getNVertex()
{
    return N_vertex;
}

int Data::getNEdge()
{
    return N_edges;
}

int Data::getEdgeCapacity(int v1, int v2)
{
    return capacity[v1][v2]; 
}

int Data::getInitialNode(){
    return initial_node;
}

int Data::getEndNode(){
    return end_node;
}

int Data::getMaxFlow(int i){
    return max_possible_flow[i];
}

std::unordered_set<int> Data::getStartNodes(){
    return start_nodes;
}