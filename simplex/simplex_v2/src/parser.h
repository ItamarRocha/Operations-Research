#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>

std::string StringRemoveBorders(std::string str);
std::vector<std::string> StringSplit(std::string str);

void print_table(std::map < std::string, std::map <std::string,long double>> Variables);
void show_tableau(std::vector<std::vector<long double>> tableau);

std::vector<std::string> get_number_of_variables(std::map < std::string, std::map <std::string,long double>> Variables);
std::vector< std::vector<long double> > get_numerical_table(int n_artificial_var,int both_case, int n_slack_var,std::map < std::string ,std::map <std::string,long double >> Variables , std::map <int,int> Constraints, std::map <std::string,long double> RHS);
std::vector< std::vector<long double> > get_tableau(std::string filename);

#endif