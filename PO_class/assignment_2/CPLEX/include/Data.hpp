#ifndef _DATA_H_
#define _DATA_H_

#include <vector>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <chrono>

#define N_MODES 3

using namespace std::chrono;

class Data
{
    private:

        int N_tasks;
        int N_days;
        int penalty;
        int max_duration;
        std::vector<std::vector<int>> costs_matrix;
        std::vector<std::vector<int>> days_taken_matrix;
        std::vector<std::pair<int, int>> precedences;

    public:

        Data(char* filePath);

        int getNTasks();

        int getNDays();

        int getPenalty();

        std::vector<std::pair<int, int>> getPrecedences();

        int getCost(int i, int k);

        int getDaysTaken(int i, int k);

        int getMaxDuration();
};

#endif

