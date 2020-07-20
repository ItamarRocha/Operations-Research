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

// EXEMPLO ADAPTADO
// Usado somente pra testes, não é compilado enquanto estiver com a extensão .cc

#include "ortools/graph/min_cost_flow.h"
#include "ortools/base/logging.h"
#include "data.h"

namespace operations_research {
struct Arc {
    std::pair<NodeIndex, NodeIndex> nodes;
    FlowQuantity capacity;
    FlowQuantity unit_cost;
};

void SolveMinCostFlow(Data &data) {
    // Alocando o vetor dos nos, que sao um par da forma (indice, oferta/demanda)
    std::vector<std::pair<NodeIndex, FlowQuantity>> supplies(data.N_vertex, {0,0});

    int arc_index = 0;
    int max_flow = 0;

    // Colocando os indices em cada um dos nos
    for(int i = 0; i < data.N_vertex; i++)
        supplies[i].first = i;

    // Atribuindo a oferta/demanda F para os nos inicial e final
    supplies[data.initial_node].second = data.max_possible_flow[data.initial_node];
    supplies[data.end_node].second = -data.max_possible_flow[data.initial_node];

    // Alocando o vetor de arcos
    std::vector<Arc> arcs(data.N_edges + 1, {{0 ,0}, 0, 0});

    // Percorrendo a matriz custos e guardando os arcos
    for (int i = 0; i < data.capacity.size(); i++){
        for (int j = 0; j < data.capacity[i].size(); j++){
            if(data.capacity[i][j])
                arcs[arc_index++] = {{i, j}, data.capacity[i][j], 0};
        }
    }

    // Definindo o arco imaginário
    arcs[arc_index++] = {{data.initial_node, data.end_node}, 999999, 1};

    // Prints para debug
    // for(int i = 0; i < supplies.size(); i++){
    //     std::cout << "Node " << supplies[i].first << " supply: " << supplies[i].second << "\n";
    // }
    // for(int i = 0; i < arcs.size(); i++){
    //     std::cout << "Arc " << arcs[i].nodes.first << " -> " << arcs[i].nodes.second <<  " CA: " << arcs[i].capacity  <<  " CO: " << arcs[i].unit_cost << "\n";
    // }

    StarGraph graph(supplies.size(), arcs.size());
    MinCostFlow min_cost_flow(&graph);
    for (const auto &it : arcs) {
        ArcIndex arc = graph.AddArc(it.nodes.first, it.nodes.second);
        min_cost_flow.SetArcCapacity(arc, it.capacity);
        min_cost_flow.SetArcUnitCost(arc, it.unit_cost);
    }
    for (const auto &it : supplies) {
        min_cost_flow.SetNodeSupply(it.first, it.second);
    }

    LOG(INFO) << "Solving min cost flow with: " << graph.num_nodes()
                << " nodes, and " << graph.num_arcs() << " arcs.";

    min_cost_flow.Solve();

    FlowQuantity total_flow_cost = min_cost_flow.GetOptimalCost();
    std::cout << "Arc   : Flow / Capacity / Cost\n";
    for (int i = 0; i < arcs.size(); ++i) {
        std::cout << graph.Tail(i) << " -> " << graph.Head(i) << ": "
                << min_cost_flow.Flow(i) << " / " << min_cost_flow.Capacity(i)
                << " / " << min_cost_flow.UnitCost(i) << "\n";
    }

    // Calculando o fluxo maximo
    for(int i = 0; i < arcs.size(); ++i) {
        if(arcs[i].nodes.second == data.end_node && !arcs[i].unit_cost)
            max_flow += min_cost_flow.Flow(i);
    }
    std::cout << "\nMax flow: " << max_flow << "\n";
}
}  // namespace operations_research

int main(int argc, char **argv) {
    google::InitGoogleLogging(argv[0]);
    FLAGS_logtostderr = 1;
    Data data = Data(argv[1]);
    operations_research::SolveMinCostFlow(data);
    return EXIT_SUCCESS;
}
