#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <unordered_map>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include "MacroDef.h"
#include "Symbol.h"

class Assembler
{
private:
    std::vector<Semantic *> * lines;

    std::vector<MacroDef *> macroList;
    std::unordered_map<std::string, MacroDef *> macroTable;
    std::unordered_map<std::string, Symbol *> symbolTable;

    std::string output;

    std::vector<unsigned char> assembleCode;

    int lineCounter;
    //int programCounter;

    int assemblerError;

    std::string macroExpandParams(std::vector<Symbol *> params, MacroDef * macroThis);
    int macroExpandParamsDoDaniel(MacroCall * macrocall, int k);

    void replaceAll(std::string& str, const std::string& from, const std::string& to);

    void assembleStep2();
    void assembleStep1();

    std::vector<unsigned char> assembleByteCode(Semantic * line);


public:
    Assembler(std::vector<Semantic *> * lines);

    //Método principal para chamar o pré processador
    std::string preproccess();
    //Método principal para chamar o montador (PRÉPROCESSADOR PRECISA SER CHAMADO ANTES)
    std::vector<unsigned char> assemble();

    void init(bool willExecute);
    int preproccessDoDaniel (std::vector<Semantic *> * lines, int k);

};

#endif // ASSEMBLER_H
