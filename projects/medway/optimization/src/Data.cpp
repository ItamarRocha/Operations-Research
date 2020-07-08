#include "../include/Data.hpp"
#include <stdlib.h>

Data::Data(char* fileMatrix, char* fileWeights, char* fileRows, char* fileColumns, char* lower_bounds)
{
    rows_number = 0;
    columns_number = 0;

    // 
    // READING THE ROWS NAME (STUDENTS) FILE
    // 
    
    std::ifstream r (fileRows, std::ios::in);

    if(!r)
    {
        printf("Problem reading rows.\n");
        exit(1);
    }

    std::string input_string;

    while(std::getline(r, input_string)){
        rows.push_back(input_string);
        rows_number++;
    }

    r.close();
    
    // 
    // READING THE COLUMNS NAME (ADVISOR) FILE
    // 
    
    std::ifstream c (fileColumns, std::ios::in);

    if(!c)
    {
        printf("Problem reading columns.\n");
        exit(1);
    }

    while(std::getline(c, input_string)){
        columns.push_back(input_string);
        columns_number++;
    }

    c.close();

    // 
    // READING THE COMPATIBILITY MATRIX FILE
    // 
    
    std::ifstream fm (fileMatrix, std::ios::in);

    if(!fm)
    {
        printf("Problem reading compatibility_matrix.\n");
        exit(1);
    }

    compatibility_matrix = std::vector< std::vector <int>> (rows_number, std::vector<int>(columns_number,0));

    int i = 0;
    while(std::getline(fm, input_string)){
        auto str_vec = stringSplit(input_string);

        for(int j = 0; j < str_vec.size(); j++){
            compatibility_matrix[i][j] = std::stoi(str_vec[j]);
            //std::cout << " " << str_vec[j] << std::endl;
        }
        //std::cout << std::endl;
        i++;
    }

    fm.close();

    // 
    // READING THE WEIGHTS FILE
    // 
    
    std::ifstream fw (fileWeights, std::ios::in);

    if(!fw)
    {
        printf("Problem reading weights.\n");
        exit(1);
    }

    while(std::getline(fw, input_string)){
        time_weights.push_back(std::stoi(input_string));
        //std::cout << input_string << std::endl;
    } 
    fw.close();

    // 
    // READING THE LOWER BOUND // INITIAL VALUES TO TIME AND NUMBER OF STUDENTS
    // 
    //     

    std::ifstream lb (lower_bounds, std::ios::in);

    if(!lb)
    {
        printf("Problem reading lower bounds.\n");
        exit(1);
    }

    // note for me: dont define the size and fill it with zeros if you are goint to
    // push values into a vector dumbass ;-; 

    while(std::getline(lb, input_string)){
        auto str_vec = stringSplit(input_string);
        //std::cout << str_vec.size() << std::endl;
        NFixedStudents.push_back(std::stoi(str_vec[0]));
        LowerBoundTime.push_back(std::stoi(str_vec[1]));
        //std::cout << str_vec[0] << " <- students || time -> " << str_vec[1] << std::endl;
    }
    //std::cout << NFixedStudents[0] << std::endl;
    lb.close();
}

std::string Data::stringRemoveBorders(std::string str)
{
    while (str[0] == ' ' || str[0] == ',')
    {
        str.erase(str.begin());
    }
    while (*(str.end() - 1) == ' ' || *(str.end() - 1) == ',')
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
            if (c == ' ' || c == ',')
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

int Data::getNRows(){
    return rows_number;
}

int Data::getNColumns(){
    return columns_number;
}

std::string Data::getRowName(int index){
    return rows[index];
}
std::string Data::getColumnName(int index){
    return columns[index];
}

int Data::getCompatibility(int i, int j){
    return compatibility_matrix[i][j];
}

int Data::getTimeWeight(int i){
    return time_weights[i];
}

int Data::getLowerBoundTime(int assessor_index){
    return LowerBoundTime[assessor_index];
}

int Data::getNFixedStudents(int assessor_index){
    return NFixedStudents[assessor_index];
}