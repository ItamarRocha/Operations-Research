// Copyright 2010-2018 Google LLC
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

#include <iostream>
#include "ortools/linear_solver/linear_solver.h"
#include "assignment_data.h"

namespace operations_research {
struct Job {
	int duration[3];
	int cost[3] = {};
    MPVariable* choosed[3];
};

void SolveJobPlanning(Data &data) {
	// [START solver]
	MPSolver solver("MCFP", MPSolver::GLOP_LINEAR_PROGRAMMING);
	// [END solver]

	// [START variables]
	const double infinity = solver.infinity();
	char var_name[10];

    // Alocando o vetor de arcos
    Job jobs[data.jobs_.size()];

	for (int i = 0; i < data.jobs_.size(); i++){
        for (int j = 0; j < 3; j++){
			jobs[i] = {{i, j}, solver.MakeIntVar(0.0, data.capacity[i][j], var_name), 0};
			solver.MakeBoolVar()
        }
    }

    // Percorrendo a matriz custos e guardando os arcos
    for (int i = 0; i < data.jobs_.size(); i++){
        for (int j = 0; j < 3; j++){
			sprintf(var_name, "x%d_%d", i+1, j+1);
			jobs[i] = {{i, j}, solver.MakeIntVar(0.0, data.capacity[i][j], var_name), 0};
        }
    }

    // Definindo o arco imaginário
	for(auto node : data.start_nodes){
		sprintf(var_name, "x%d_%d", node+1, data.end_node+1);
		arcs[arc_index++] = {{node, data.end_node}, solver.MakeIntVar(0.0, infinity, var_name), 1};
	}

	LOG(INFO) << "Number of variables = " << solver.NumVariables();
	// [END variables]

	// [START constraints]
	MPConstraint* constraints[data.N_vertex];
	for (int i = 0; i < data.N_vertex; i++){
		constraints[i] = solver.MakeRowConstraint(supplies[i], supplies[i]);
		for (int j = 0; j < data.N_edges + data.start_nodes.size(); j++){
			if (arcs[j].nodes.first == i)
				constraints[i]->SetCoefficient(arcs[j].flow_var, 1);
			if (arcs[j].nodes.second == i)
				constraints[i]->SetCoefficient(arcs[j].flow_var, -1);
		}
	}
	LOG(INFO) << "Number of constraints = " << solver.NumConstraints();
	// [END constraints]

	// [START objective]
	// Objective function: Min SUM Xij*Cij.
	MPObjective* const objective = solver.MutableObjective();
	for (int i = 0; i < data.N_edges + data.start_nodes.size(); i++)
		objective->SetCoefficient(arcs[i].flow_var, arcs[i].cost);
	objective->SetMinimization();
	// [END objective]

	// [START solve]
	std::chrono::high_resolution_clock::time_point t0, t1;

	t0 = std::chrono::high_resolution_clock::now();
	const MPSolver::ResultStatus result_status = solver.Solve();
	t1 = std::chrono::high_resolution_clock::now();
	// Check that the problem has an optimal solution.
	if (result_status != MPSolver::OPTIMAL) {
		LOG(FATAL) << "The problem does not have an optimal solution!";
	}
	// [END solve]
	
	// [START print_solution]
	int max_flow = 0;
	LOG(INFO) << "Solution:";
	LOG(INFO) << "Optimal objective value = " << objective->Value();
	LOG(INFO) << "Solved in: " << std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() << " us"; 

	for(int i = 0; i < data.N_edges + data.start_nodes.size(); i++){
		if(arcs[i].flow_var->solution_value() > 0)
			LOG(INFO) << arcs[i].flow_var->name() << " = " << arcs[i].flow_var->solution_value();
		if(arcs[i].nodes.second == data.end_node && !arcs[i].cost)
			max_flow += arcs[i].flow_var->solution_value();
	}

	LOG(INFO) << "Max flow: " << max_flow;
	// [END print_solution]
}
}  // namespace operations_research

int main(int argc, char** argv) {
	google::InitGoogleLogging(argv[0]);
	FLAGS_logtostderr = 1;
	Data data = Data(argv[1]);
	
	operations_research::SolveJobPlanning(data);
	return EXIT_SUCCESS;
}
// [END program]
