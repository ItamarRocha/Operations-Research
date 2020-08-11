#ifndef _DATA_H_
#define _DATA_H_

#include <vector>
#include <iostream>

struct ExecutionType{
    int duration;
    int cost;
};

struct Job{
    ExecutionType normal;
    ExecutionType fast;
    ExecutionType fastest;
};

class Data{
    public:
        int deadline_;
        int fine_value_;
        std::vector<std::pair<int, int>> precedences_;
        std::vector<Job> jobs_;

        Data(char* file_path);
        void print();
};

#endif

