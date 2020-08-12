#include "../include/Data.hpp"

Data::Data(char* filePath)
{
    FILE* f = fopen(filePath, "r");
    
    if(!f)
    {
        printf("Problem while reading instance. Check file.\n");
        exit(1);
    }

    if(fscanf(f, "%d", &N_tasks) != 1)
    {
        printf("Problem while reading instance. Check tasks.\n");
        exit(1);
    }

    if(fscanf(f, "%d", &N_days) != 1)
    {
        printf("Problem while reading instance. Check Days.\n");
        exit(1);
    }

    if(fscanf(f, "%d", &penalty) != 1)
    {
        printf("Problem while reading instance. Check the penalty.\n");
        exit(1);
    }

    costs_matrix = std::vector<std::vector <int>> (N_tasks, std::vector<int>(N_MODES));
    days_taken_matrix = std::vector<std::vector<int>> (N_tasks, std::vector<int>(N_MODES));
    max_duration = 0;

    for(int i = 0; i < N_tasks; i++)
    {
        costs_matrix[i][0] = 0;
        if(fscanf(f, "%d %d %d %d %d", &days_taken_matrix[i][0], &days_taken_matrix[i][1], &costs_matrix[i][1], &days_taken_matrix[i][2], &costs_matrix[i][2]) != 5) //returns the number of sucessfully assigned input items
        {
            printf("Problem while reading instance.\n");
            exit(1);
        }
        //std::cout << days_taken_matrix[i][2] << std::endl;
        max_duration += days_taken_matrix[i][0];
    }
    //std::cout << max_duration << std::endl;
    int node1;
    int node2;

    while(fscanf(f,"%d %d", &node1, &node2) != EOF){
        precedences.push_back(std::make_pair(node1, node2));
    }

    fclose(f);
}

int Data::getNTasks(){
    return N_tasks;
}

int Data::getNDays(){
    return N_days;
}

int Data::getPenalty(){
    return penalty;
}

std::vector<std::pair<int,int>> Data::getPrecedences(){
    return precedences;
}

int Data::getCost(int i, int k){
    return costs_matrix[i][k];
}

int Data::getDaysTaken(int i, int k){
    return days_taken_matrix[i][k];
}

int Data::getMaxDuration(){
    return max_duration;
}