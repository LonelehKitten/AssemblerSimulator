#ifndef MACRODEF_H
#define MACRODEF_H

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <utility>

#include "../Utils.h"
#include "../analyzer/Semantic.h"

class MacroDef
{
private:
    std::string name;   // nome de chamada da macro
    std::vector<std::string> args; // #00 #01 ...
    std::vector<Semantic *> * macroText; // texto da definição da macro
    
public:
    // constructor
    MacroDef(std::string name, std::vector<std::string> args);

    // getters
    std::string getName();
    std::vector<std::string> getArgs();
    std::vector<Semantic *> * getText();

    // setters
    void setText(Semantic * line);

    // métodos
    std::string macroLine();
    void updateMacro(std::vector<std::string> args);

    std::vector<std::pair<std::string, std::string> *> * getParamValuePairs(MacroCall * macrocall);

};

#endif
