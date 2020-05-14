#include "parser.h"
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

/*void print_map(std::map<std::string,auto> map){

    for (auto const& x : map){

    std::cout << x.first  // string (key)
              << " -> "
              << x.second // string's value 
              << std::endl ;
    
    }
}*/

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

void show_tableau(std::vector<std::vector<double>> tableau){
    for (int i = 0; i < tableau.size(); i++) { 
        for (int j = 0; j < tableau[i].size(); j++) 
            cout << tableau[i][j] << " "; 
        cout << endl; 
    } 
}


std::vector<std::string> get_number_of_variables(std::map < std::string, std::map <std::string, double>> Variables){
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
    return var;
}

std::vector< std::vector<double> > get_numerical_table(int n_artificial_var,int both_case, int n_slack_var,std::map < std::string ,std::map <std::string,double >> Variables , std::map <int,int> Constraints, std::map <std::string, double> RHS){
    std::vector<std::string> columns_name = get_number_of_variables(Variables);
    int number_of_variables = columns_name.size();
    int number_of_equations = Variables.size();
    

    std::vector<std::vector<double>> tableau(number_of_equations, std::vector<double> (number_of_variables + n_slack_var + n_artificial_var + both_case*2 ,0)); // +1 in reference to RHS
    int i = 0;
    int k = 0;
    bool reset = 1;
    for(auto & rows : Variables) { //rows
            //cout << rows.first << " contains: " << endl;
        for(k = 0; k < number_of_variables - 1; k++) { // columns
            tableau[i][k] = Variables[rows.first][columns_name[k]];
            //cout << columns_name[k] << " = " << tableau[i][k] << endl;
        }
        
        // this tablear[0].size() - 1 is equivalent to the number of columns, the index of the last one
        tableau[i][tableau[0].size() - 1] = Variables[rows.first][columns_name[number_of_variables - 1]]; // number_of_variables -1 is to find the RHS values
        // puts in the last tableau column which is suposed to be the RHS
        i++;
        //cout << tableau[0].size() - 1 << endl;
    }
    
    std::vector<int> VB;
    VB.push_back(0); // adding the cost line
    for(auto & constr : Constraints){
        /*show_tableau(tableau);
        cout << endl;
        cout << k << endl;
        cout << constr.first << endl;*/
        if(k == tableau[0].size() - 1)
            break;
        if(constr.second == 0){
            continue;
        }else if(constr.second == 1){
            tableau[constr.first][k] = 1;
            VB.push_back(k+1);    
        }else if(constr.second == 2){
            tableau[constr.first][k] = 1;
            tableau[0][k] = -10000;
            VB.push_back(k+1);
        }else if(constr.second == 3){
            tableau[constr.first][k] = -1;
            VB.push_back(k+2);
            tableau[constr.first][k + 1] = 1;
            tableau[0][k+1] = -10000;
            k++;
        }
        k++;
    }

    for(int j = 0; j < VB.size() ; j++){
        cout << VB[j] << endl;
    }

    for(int j = 0 ; j < tableau[0].size(); j++){
        tableau[0][j] *= -1;
    }

    return tableau;
}


std::vector< std::vector<double> > get_tableau(std::string filename){

	ifstream f (filename, ios::in);

    map<int,int> Constraints;
    int n_artificial_var = 0;
    int n_slack_var = 0;
    int both_case = 0;

	std::string string;

    int i = 0;
    /*
        ROWS AND CONSTRAINTS
    */
    int row_number = 0;
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

            Constraints[row_number] = 0;
            row_number++;
        }else if(str_vec[0] == "L"){
            n_slack_var++;
            Constraints[row_number] = 1;
            row_number++;
        }else if(str_vec[0] == "E"){
            n_artificial_var++;
            Constraints[row_number] = 2;
            row_number++;
        }else if(str_vec[0] == "G"){
            both_case++;
            Constraints[row_number] = 3;
            row_number++;
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
        }
    }

    std::map < std::string, double> RHS;

    while(std::getline(f,string)){
        if(!string.compare("BOUNDS") or !string.compare("ENDATA"))
            break;
        

        auto str_vec = StringSplit(string);

        /*
            Doing the iteration in this way it will only grep the keys of each column
            and the value of the key is right next to it, so we only need to iter within
            the keys.
        */
        for(int j = 1; j < str_vec.size(); j+= 2){ 
            
            Variables[str_vec[j]][str_vec[0]] = std::stod(str_vec[j+1]);
            //std:: cout << std::stod(str_vec[j+1]) << std::endl;
        }        
    }
    /*print_map(RHS);
    print_table(Variables);
    print_map(Constraints);*/

    std::vector< std::vector<double> > tableau = get_numerical_table(n_artificial_var,both_case,n_slack_var,Variables ,Constraints,RHS);
    
    show_tableau(tableau);
    // MAS DE BOA PQ OQ VC N MODIFICA ELE JA SETA COMO 0, AGORA É SÓ VER A PARADA DE MAXIMIZAR
    // MINIMIZAR E BOTAR AS PENALIDADES E VARIAVEIS DE FOLGA EITA MZRA QUE TEM COISA VIU
	/* tem que passar pra um array agora essa mzra kkkkk */
    // fazer meio que um map pra associar cada coluna ao numero dela na outra kk
    return tableau;
}