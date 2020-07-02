#ifndef SOLVER_H
#define SOLVER_H
#include <vector>

class Solver{
  public:
    std::vector< std::vector<long double>> tableau;
    
    int get_pivot_column();
    int get_pivot_row();
    int check_optimality();
    int solve();
    void print_results();
};



#endif