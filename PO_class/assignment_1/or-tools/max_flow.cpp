// Copyright 2018 Google LLC
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "ortools/graph/max_flow.h"
#include "ortools/base/logging.h"
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <unordered_set>
#include <iostream>

class Data
{
    private:

        int N_vertex;
        int N_edges;
        int initial_node;
        int end_node;
        std::vector<int> max_possible_flow;
        std::unordered_set<int> start_nodes;

    public:

        std::vector<std::vector<int>> capacity; //matrix of the values

        Data(char* filePath){
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

            // 
            // Aqui a gente ta iterando por cada no de entrada e somando a capacidade maxima de saida
            // de cada um desses nós para cada no transshipment e somando pra ver qual a capacidade
            // máxima do nó de entrada.
            //
            for (int i = 0; i < start_nodes.size(); i++)
            {
                for(int j = 0; j < N_vertex; j++){
                    max_possible_flow[i] += capacity[i][j];
                }
            }

            fclose(f);
        }

        void pfcm_convertion(){
            // aqui a gente adiciona um caminho indo dos nós de entrada pro nó de saída.
            // como colocado na modelagem, só vai passar por aqui oq nao passar pela network
            // normal, já que o custo pra passar por ela é zero e o custo pra passar por aqui
            // vai ser maior que zero
            for(auto start_node : start_nodes){
                capacity[start_node][end_node] = max_possible_flow[start_node];
            }
        }

        int getNVertex(){
            return N_vertex;
        }

        int getNEdge(){
            return N_edges;
        }
};

namespace operations_research {
void SolveMaxFlow(Data &data) {
    std::vector<std::pair<std::pair<NodeIndex, NodeIndex>, FlowQuantity>> arcs;
    for (int i = 0; i < data.capacity.size(); i++){
        for (int j = 0; j < data.capacity[i].size(); j++){
            arcs.push_back({{i, j}, data.capacity[i][j]});
        }
    }
    StarGraph graph(data.getNVertex(), arcs.size());
    MaxFlow max_flow(&graph, 0, data.getNVertex() - 1);
    for (const auto& it : arcs) {
        ArcIndex arc = graph.AddArc(it.first.first, it.first.second);
        max_flow.SetArcCapacity(arc, it.second);
    }

    LOG(INFO) << "Solving max flow with: " << graph.num_nodes() << " nodes, and "
            << graph.num_arcs() << " arcs.";

    // Find the maximum flow between node 0 and node 4.
    max_flow.Solve();
    if (MaxFlow::OPTIMAL != max_flow.status()) {
        LOG(FATAL) << "Solving the max flow is not optimal!";
    }
    FlowQuantity total_flow = max_flow.GetOptimalFlow();
    LOG(INFO) << "Maximum flow: " << total_flow;
    LOG(INFO) << "";
    LOG(INFO) << " Arc  : Flow / Capacity";
    for (int i = 0; i < arcs.size(); ++i) {
        LOG(INFO) << graph.Tail(i) << " -> " << graph.Head(i) << ": "
                  << max_flow.Flow(i) << " / " << max_flow.Capacity(i);
    }
}
}  // namespace operations_research

int main(int argc, char** argv) {
    google::InitGoogleLogging(argv[0]);
    FLAGS_logtostderr = 1;
    Data data = Data(argv[1]);
    operations_research::SolveMaxFlow(data);
    return EXIT_SUCCESS;
}