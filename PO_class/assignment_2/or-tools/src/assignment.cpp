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
#include <fstream>
#include "ortools/linear_solver/linear_solver.h"
#include "data.h"

namespace operations_research {
struct JobExecutionVar
{
	MPVariable* normal;
	MPVariable* fast;
	MPVariable* fastest;
};

void SolveJobPlanning(Data &data) {
	// [START solver]
	MPSolver solver("MCFP", MPSolver::GLOP_LINEAR_PROGRAMMING);
	// [END solver]

	// [START variables]
	
	// Variaveis auxiliares
	const double infinity = solver.infinity();
	char var_name[10];
	int max_duration = 0;

	for (int i = 0; i < data.jobs_.size(); i++)
		max_duration += data.jobs_[i].normal.duration;

	MPVariable* alfa = solver.MakeIntVar(0.0, max_duration, "alfa");
	MPVariable* beta = solver.MakeIntVar(0.0, max_duration, "beta");

	MPVariable* X[data.jobs_.size()][data.jobs_.size()];
	for (int i = 0; i < data.jobs_.size(); i++){
        for (int j = 0; j < data.jobs_.size(); j++){
			sprintf(var_name, "x[%d][%d]", i, j);
			X[i][j] = solver.MakeBoolVar(var_name);
        }
    }

	JobExecutionVar E[data.jobs_.size()];
	for (int i = 0; i < data.jobs_.size(); i++){
		sprintf(var_name, "m[%d][n]", i);
		E[i].normal = solver.MakeBoolVar(var_name);

		sprintf(var_name, "m[%d][a]", i);
		E[i].fast = solver.MakeBoolVar(var_name);

		sprintf(var_name, "m[%d][f]", i);
		E[i].fastest = solver.MakeBoolVar(var_name);
    }

	MPVariable* C[data.jobs_.size()];
	for (int i = 0; i < data.jobs_.size(); i++){
		sprintf(var_name, "c[%d]", i);
		C[i] = solver.MakeIntVar(0.0, max_duration, var_name);
    }

	LOG(INFO) << "Number of variables = " << solver.NumVariables();
	// [END variables]

	// [START constraints]

	// All nodes should have only 2 connections (in and out)
	for (int i = 0; i < data.jobs_.size(); i++){
		MPConstraint* one_in = solver.MakeRowConstraint(1.0, 1.0);
		MPConstraint* one_out = solver.MakeRowConstraint(1.0, 1.0);

		for (int j = 0; j < data.jobs_.size(); j++){
			one_in->SetCoefficient(X[i][j], 1.0);
			one_out->SetCoefficient(X[j][i], 1.0);
		}
	}

	// The nodes cant connect to themselves
	for (int i = 0; i < data.jobs_.size(); i++){
		MPConstraint* connect_to_self = solver.MakeRowConstraint(0.0, 0.0);
		connect_to_self->SetCoefficient(X[i][i], 1.0);
	}
	// MPConstraint* imaginary_zero = solver.MakeRowConstraint(0, 0);
	// imaginary_zero->SetCoefficient(C[0], 1);

	MPConstraint* first_arc = solver.MakeRowConstraint(1.0, 1.0);
	first_arc->SetCoefficient(X[0][data.precedences_[0].first], 1.0);

	MPConstraint* last_arc = solver.MakeRowConstraint(1.0, 1.0);
	last_arc->SetCoefficient(X[data.precedences_[data.precedences_.size()-1].second][0], 1.0);

	// Cj >= Ci + Dj - (1-Xij)*M    ->    Cj - Ci - Dj -M*Xij >= -M
	// for (int i = 0; i < data.jobs_.size(); i++){
	// 	for (int j = 1; j < data.jobs_.size(); j++){
	// 		if (i != j){
	// 			MPConstraint* end_time = solver.MakeRowConstraint(-max_duration, infinity);

	// 			end_time->SetCoefficient(C[j], 1);
	// 			end_time->SetCoefficient(C[i], -1);

	// 			// Dj
	// 			end_time->SetCoefficient(E[j].normal, -data.jobs_[j].normal.duration);
	// 			end_time->SetCoefficient(E[j].fast, -data.jobs_[j].fast.duration);
	// 			end_time->SetCoefficient(E[j].fastest, -data.jobs_[j].fastest.duration);
				
	// 			end_time->SetCoefficient(X[i][j], -max_duration);
	// 		}
	// 	}
	// }

	// Cj >= Ci + Dj - (1-Xij)*M
	for (int i = 0; i < data.jobs_.size(); i++){
		for (int j = 1; j < data.jobs_.size(); j++){
			if (i != j){
				LinearExpr current_c;

				current_c =  LinearExpr(C[i])
							+LinearExpr(E[j].normal) * data.jobs_[j].normal.duration
							+LinearExpr(E[j].fast) * data.jobs_[j].fast.duration
							+LinearExpr(E[j].fastest) * data.jobs_[j].fastest.duration
							+LinearExpr(X[i][j]) * max_duration
							-max_duration;

				solver.MakeRowConstraint(LinearExpr(C[j]) >= current_c);
			}
		}
	}

	// Assures that each job will have only 1 execution method
	for (int i = 0; i < data.jobs_.size(); i++){
		MPConstraint* execution_mode = solver.MakeRowConstraint(1.0, 1.0);

        execution_mode->SetCoefficient(E[i].normal, 1.0);
		execution_mode->SetCoefficient(E[i].fast, 1.0);
		execution_mode->SetCoefficient(E[i].fastest, 1.0);
	}

	// Precedences
	// Cj - Ci >= 1
	for (int i = 0; i < data.precedences_.size(); i++){
		MPConstraint* precedence_constraint = solver.MakeRowConstraint(1.0, max_duration);

		precedence_constraint->SetCoefficient(C[data.precedences_[i].second], 1.0);
		precedence_constraint->SetCoefficient(C[data.precedences_[i].first], -1.0);
	}

	// alfa = Sum(durations)    ->    alfa - Sum(durations) = 0
	MPConstraint* alfaValue = solver.MakeRowConstraint(0.0, 0.0);
	alfaValue->SetCoefficient(alfa, -1);
	for (int i = 0; i < data.jobs_.size(); i++){
        alfaValue->SetCoefficient(E[i].normal, data.jobs_[i].normal.duration);
		alfaValue->SetCoefficient(E[i].fast, data.jobs_[i].fast.duration);
		alfaValue->SetCoefficient(E[i].fastest, data.jobs_[i].fastest.duration);
	}

	// beta >= alfa - deadline    ->    beta - alfa >= -deadline
	MPConstraint* betaValue = solver.MakeRowConstraint(-data.deadline_, infinity);
	betaValue->SetCoefficient(beta, 1.0);
	betaValue->SetCoefficient(alfa, -1.0);

	LOG(INFO) << "Number of constraints = " << solver.NumConstraints();
	// [END constraints]

	// [START objective]
	// Objective function: MIN Sum(Xij*Cij) + beta*penalty.
	MPObjective* const objective = solver.MutableObjective();

	for (int i = 0; i < data.jobs_.size(); i++){
		objective->SetCoefficient(E[i].normal, data.jobs_[i].normal.cost);
		objective->SetCoefficient(E[i].fast, data.jobs_[i].fast.cost);
		objective->SetCoefficient(E[i].fastest, data.jobs_[i].fastest.cost);
	}

	objective->SetCoefficient(beta, data.penalty_);

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
	LOG(INFO) << "Solution:";
	LOG(INFO) << "Optimal objective value = " << objective->Value();
	LOG(INFO) << "Solved in: " << std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() << " us"; 

	for(int i = 0; i < data.jobs_.size(); i++)
		LOG(INFO) << C[i]->name() << " = " << C[i]->solution_value();

	for(int i = 0; i < data.jobs_.size(); i++){
		for(int j = 0; j < data.jobs_.size(); j++){
			if(X[i][j]->solution_value() > 0.5)
				LOG(INFO) << X[i][j]->name() << " = " << X[i][j]->solution_value();
		}
	}

	for (int i = 1; i < data.jobs_.size(); i++){
		if(E[i].normal->solution_value() > 0.1){
			LOG(INFO) << i << " method = normal";
			continue;
		}

		if(E[i].fast->solution_value() > 0.1){
			LOG(INFO) << i << " method = fast";
			continue;
		}

		if(E[i].fastest->solution_value() > 0.1)
			LOG(INFO) << i << " method = fastest";
	}

	LOG(INFO) << alfa->name() << " = " << alfa->solution_value();
	LOG(INFO) << beta->name() << " = " << beta->solution_value();

	std::string model;
	solver.ExportModelAsLpFormat(true, &model);

	std::ofstream file ("test.txt", std::ofstream::out);
	file << model;
	file.close();

	// LOG(INFO) << "Max flow: " << max_flow;
	// [END print_solution]
}
}  // namespace operations_research

int main(int argc, char** argv) {
	google::InitGoogleLogging(argv[0]);
	FLAGS_logtostderr = 1;
	Data data = Data(argv[1]);
	data.print();
	
	operations_research::SolveJobPlanning(data);
	return EXIT_SUCCESS;
}
// [END program]
