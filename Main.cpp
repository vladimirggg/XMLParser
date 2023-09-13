#include "Parser.h"
#include <fstream>
#include <exception>

std::string collectData(){
    std::string filename, info;
    std::cout<<"Enter filename: ";
    std::getline(std::cin,filename);
    try
    {
        std::ifstream file(filename, std::ios::in);
        if(!file.is_open())
            throw std::runtime_error("Error loading the file");

        char ch;
        while (file.get(ch)) 
            info += ch;

        file.close();
 
    }
    catch(const std::runtime_error& e)
    {
        std::cerr << e.what() << '\n';
    }

    return info;
}
int main()
{
    std::string info = collectData();

    XMLParser data;
    data.createUsing(info);
    data.menu();
    return 0;
}