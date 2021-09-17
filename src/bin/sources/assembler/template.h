#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include "Semantic.h"


struct label
{
    std::string name;
    std::string valor;
};

class macroDef
{
private:
    std::string name;   // nome de chamada da macro
    std::vector<std::string> args; // #00 #01 ...
    std::string macroText; // texto da definição da macro
    
public:
    // constructor
    macroDef(std::string name, std::vector<std::string> args);

    // getters
    std::string getName();
    std::vector<std::string> getArgs();
    std::string getText();

    // setters
    void setText(std::string line);

    // métodos
    std::string macroLine();
    void updateMacro(std::vector<std::string> args);

};

#endif