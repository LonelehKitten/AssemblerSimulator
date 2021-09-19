#ifndef MACRODEF_H
#define MACRODEF_H

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include "../analyzer/Semantic.h"

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
    std::vector<Semantic *> macroText; // texto da definição da macro
    
public:
    // constructor
    macroDef(std::string name, std::vector<std::string> args);

    // getters
    std::string getName();
    std::vector<std::string> getArgs();
    std::vector<Semantic *> & getText();

    // setters
    void setText(Semantic * line);

    // métodos
    std::string macroLine();
    void updateMacro(std::vector<std::string> args);

};

#endif