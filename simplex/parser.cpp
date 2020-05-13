#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
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

int get_number_of_variables(std::map < std::string, std::map <std::string, double>> Variables){
    std::vector<std::string> var;

    for(auto & rows : Variables) {
        for(auto & columns : rows.second){
            if(std::find(var.begin(), var.end(), columns.first) != var.end()){
                //true statement if present
                continue;
            }else{
                var.push_back(columns.first);
                //cout << columns.first << endl;
            }
            //cout << rows.first << " = " << columns.second << endl;
        }
    }
    return var.size();
}


std::vector< std::vector<double> > get_numerical_table(std::map < std::string ,std::map <std::string,double >> Variables , std::map <std::string,int> Constraints, std::map <std::string, double> RHS){

    int number_of_variables = get_number_of_variables(Variables);
    int number_of_equations = Variables.size();
    

    std::vector<std::vector<double>> tableau(number_of_equations, std::vector<double> (number_of_variables,0));
    int i = 0;
    int j = 0;
    for(auto & outer_map_pair : Variables) {
            //cout << outer_map_pair.first << " contains: " << endl;
        for(auto & inner_map_pair : outer_map_pair.second) {
            //tableau[i][j] = inner_map_pair.second;
            cout <<inner_map_pair.second << " ";
            j++;
        }
        i++;
        std::cout << std::endl;   

    }


    return tableau;
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

    int max_size = 0;

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
            
            Variables[str_vec[j]][str_vec[0]] = std::stod(str_vec[j+1]);
            //std:: cout << std::stod(str_vec[j+1]) << std::endl;
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
            //std:: cout << std::stod(str_vec[j+1]) << std::endl;
            //std::cout << str_vec[j] << std::endl; 
            //cout << j << endl;
        }        
    }
    /*print_map(RHS);
    print_table(Variables);
    print_map(Constraints);*/
    int a = get_number_of_variables(Variables);
    cout << a << endl;
    /*
    std::vector< std::vector<double> > tableau = get_numerical_table(Variables,Constraints,RHS);
    for(auto v : Variables)
        std::cout << v.second.find("X1").second << endl;
    */// MAS DE BOA PQ OQ VC N MODIFICA ELE JA SETA COMO 0, AGORA É SÓ VER A PARADA DE MAXIMIZAR
    // MINIMIZAR E BOTAR AS PENALIDADES E VARIAVEIS DE FOLGA EITA MZRA QUE TEM COISA VIU
	/* tem que passar pra um array agora essa mzra kkkkk */
    // fazer meio que um map pra associar cada coluna ao numero dela na outra kk
    return 0;
}