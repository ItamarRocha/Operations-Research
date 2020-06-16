#include "parser.h"
#include <iostream>

int main(int argc, char* argv[]){
  if(argc != 2){
    std::cout << "./simplex instances/intance_name" << std::endl;
    exit(1);
  }

  std::vector<std::vector<long double>> tableau = get_tableau(argv[1]); 

  show_tableau(tableau);

  exit(0);
}