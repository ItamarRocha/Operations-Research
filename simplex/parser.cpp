#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <unordered_map>
using namespace std;

std::string StringRemoveBorders(std::string str)
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

std::vector<std::string> StringSplit(std::string str)
{
    std::vector<std::string> str_vec;
    str = StringRemoveBorders(str);

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
        str = StringRemoveBorders(str);
    }

    return str_vec;
}

void print_map(std::map<std::string,auto> map){

    for (auto const& x : map){

    std::cout << x.first  // string (key)
              << " -> "
              << x.second // string's value 
              << std::endl ;
    
    }
}

void print_table(std::map < std::string, std::map <std::string, double>> Variables){
    for(auto & outer_map_pair : Variables) {
            //cout << outer_map_pair.first << " contains: " << endl;
        cout << outer_map_pair.first << "-> ";
        for(auto & inner_map_pair : outer_map_pair.second) {
            cout <<inner_map_pair.first << ": " << inner_map_pair.second << " ";
        }
        std::cout << std::endl;   

    }
}

int main(){

	ifstream f ("instances/p2.mps", ios::in);

    map<std::string,int> Constraints;


	std::string string;

    int i = 0;
    /*
        ROWS AND CONSTRAINTS
    */
    while(getline(f,string)){

        if (i <= 1){
            i++;
            continue;
        }

        if(!string.compare("COLUMNS")){
            break;
        }

        auto str_vec = StringSplit(string);
        /*
            0 -> non constraining (N)
            1 -> Less (L)
            2 -> Equal (E)
            3 -> Greater (G)
        */

        if(str_vec[0] == "N"){

            Constraints[str_vec[1]] = 0;

        }else if(str_vec[0] == "L"){

            Constraints[str_vec[1]] = 1;

        }else if(str_vec[0] == "E"){

            Constraints[str_vec[1]] = 2;

        }else if(str_vec[0] == "G"){

            Constraints[str_vec[1]] = 3;

        }else{
            printf("Error No such constraint\n");
            exit(-1);
        }

    }

    //print_map(Constraints); //print the map we just saw
        
    std::map < std::string, std::map <std::string, double>> Variables;

    while(std::getline(f,string)){

        if(!string.compare("RHS"))
            break;

        //cout<< string << endl; prints the hole line

        auto str_vec = StringSplit(string);

        /*
            Doing the iteration in this way it will only grep the keys of each column
            and the value of the key is right next to it, so we only need to iter within
            the keys.
        */
        for(int j = 1; j < str_vec.size(); j+= 2){ 
            
            Variables[str_vec[0]][str_vec[j]] = std::stod(str_vec[j+1]);
            std:: cout << std::stod(str_vec[j+1]) << std::endl;
            //std::cout << str_vec[j] << std::endl; 
            //cout << j << endl;
        }
    }

    std::map < std::string, double> RHS;

    while(std::getline(f,string)){
        if(!string.compare("BOUNDS"))
            break;
        

        auto str_vec = StringSplit(string);

        /*
            Doing the iteration in this way it will only grep the keys of each column
            and the value of the key is right next to it, so we only need to iter within
            the keys.
        */
        for(int j = 1; j < str_vec.size(); j+= 2){ 
            
            RHS[str_vec[j]] = std::stod(str_vec[j+1]);
            std:: cout << std::stod(str_vec[j+1]) << std::endl;
            //std::cout << str_vec[j] << std::endl; 
            //cout << j << endl;
        }        
    }
    print_map(RHS);
    print_table(Variables);

    /* tem que passar pra um array agora essa mzra kkkkk */
    
	return 0;
}