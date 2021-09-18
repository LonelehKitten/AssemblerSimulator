#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include "Semantic.h"

//#include "SyntaxAnalyzer.h"

#define T std::string

class Macro
{
private:
    std::string name;   // nome de chamada da macro
    std::vector<T> args; // #00 #01 ...
    std::string macroText; // texto da definição da macro
    
public:
    // constructor
    Macro();

    // getters
    std::string getName();
    std::vector<T> getArgs();
    std::string getText();

    // métodos
    std::string macroLine();
    void updateMacro(std::vector<T> args);

};

//std::vector<Macro> table;
#endif