#include "data.h"

Data::Data(char* file_path){
    FILE* f = fopen(file_path, "r");
    int n_jobs;
    int buffer[5];

    if(!f){
        std::cerr << "ERROR: Could not open file!\n";
        exit(EXIT_FAILURE);
    }

    if(fscanf(f, "%d", &n_jobs) != 1){
        std::cerr << "ERROR: Check deadline!\n";
        exit(EXIT_FAILURE);
    }

    if(fscanf(f, "%d", &deadline_) != 1){
        std::cerr << "ERROR: Check deadline!\n";
        exit(EXIT_FAILURE);
    }

    if(fscanf(f, "%d", &penalty_) != 1){
        std::cerr << "ERROR: Check fine value!\n";
        exit(EXIT_FAILURE);
    }

    jobs_.push_back({{0, 0}, {0, 0}, {0, 0}});

    while(n_jobs--){
        if(fscanf(f, "%d %d %d %d %d", &buffer[0], &buffer[1], &buffer[2], &buffer[3], &buffer[4]) != 5){
            std::cerr << "ERROR: Check jobs!\n";
            exit(EXIT_FAILURE);
        }
        jobs_.push_back({{buffer[0], 0}, {buffer[1], buffer[2]}, {buffer[3], buffer[4]}});
    }

    while(fscanf(f, "%d %d", &buffer[0], &buffer[1]) == 2)
        precedences_.push_back({buffer[0], buffer[1]});    

    fclose(f);
}

void Data::print(){
    std::cout << "Deadline: " << deadline_ << "\n";
	std::cout << "Fine value: " << penalty_ << "\n";

	std::cout << "\nJobs:\n";
	for(int i = 0; i < jobs_.size(); i++)
		std::cout << "\tNumber " << i << ":{\n"
				  << "\t\tNormal: " << jobs_[i].normal.duration << "\n"
				  << "\t\tFast: " << jobs_[i].fast.duration << " ($"<< jobs_[i].fast.cost << ")\n"
				  << "\t\tFastest: " << jobs_[i].fastest.duration << " ($"<< jobs_[i].fastest.cost << ")\n\t}\n";

	std::cout << "\nPrecedences:\n";
	for(int i = 0; i < precedences_.size(); i++)
		std::cout << precedences_[i].first << " -> " << precedences_[i].second << "\n";
}