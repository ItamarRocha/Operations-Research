#include "../include/Data.hpp"

Data::Data(char * filepath){

	std::ifstream f(filepath, std::ios::in);
	std::string line;
	std::vector<std::string> formatted_line;

	getline(f,line);

	formatted_line = stringSplit(line);

	n_items = stod(formatted_line[0]);
	bin_capacity = stod(formatted_line[1]);

	for(int i = 0; i < n_items; i++){
		getline(f,line);
		formatted_line = stringSplit(line);
		values.push_back(stod(formatted_line[0]));
		weights.push_back(stod(formatted_line[1]));
	
	}

	std::cout << formatted_line[0] << std::endl;
}

std::string Data::stringRemoveBorders(std::string str)
{
    while (str[0] == ' ')
    {
        str.erase(str.begin());
    }
    while (*(str.end() - 1) == ' ')
    {
        str.erase(str.end() - 1);
    }

    return str;
}

std::vector<std::string> Data::stringSplit(std::string str)
{
    std::vector<std::string> str_vec;
    str = stringRemoveBorders(str);

    while (str.size() != 0)
    {

        int i = 0;
        for (char c : str)
        {
            if (c == ' ')
            {
                str_vec.push_back(std::string(str.begin(), str.begin() + i));
                i++;
                break;
            }
            else if (i == str.size() - 1)
            {
                str_vec.push_back(std::string(str.begin(), str.begin() + i + 1));
                i++;
                break;
            }
            i++;
        }
        str = std::string(str.begin() + i, str.end());
        str = stringRemoveBorders(str);
    }

    return str_vec;
}

int Data::getNItems(){
	return n_items;
}

int Data::getBinCapacity(){
	return bin_capacity;
}

int Data::getItemWeight(int item){
	return weights[item];
}

int Data::getItemValue(int item){
	return values[item];
}