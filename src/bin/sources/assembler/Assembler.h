#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <unordered_map>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include "MacroDef.h"
#include "Label.h"
#include "../analyzer/Semantic.h"
#include "../analyzer/RecognitionManager.h"

class Assembler
{
private:
    std::vector<Semantic *> * lines;

    std::unordered_map<std::string, MacroDef *> macroTable;
    std::unordered_map<std::string, Label *> symbolTable;

    std::string output;

    std::vector<unsigned char> assembleCode;

    int lineCounter;
    //int programCounter;

    int assemblerError;


    std::string macroExpandParams(std::vector<Label *> params, MacroDef * macroThis);
    int macroExpandParamsDoDaniel(MacroCall * macrocall, int k);

    void replaceAll(std::string& str, const std::string& from, const std::string& to);

    void assembleStep2();

    void Assembler::assembleByteCode(Semantic * line);

public:
    Assembler(std::vector<Semantic *> * lines);

    std::string preproccess();

    void init(bool willExecute);
    int preproccessDoDaniel (std::vector<Semantic *> * lines, int k);

    void assemble();

};

#endif // ASSEMBLER_H
