#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <unordered_map>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include "MacroDef.h"
#include "../analyzer/Semantic.h"
#include "../analyzer/RecognitionManager.h"

struct label
{
    std::string name;
    std::string valor;
};

class Assembler
{

private:
    std::vector<Semantic *> * lines;
    std::vector<MacroDef *> macroTable;
    std::vector<label *> labelTable;

    std::string output;

    std::string macroExpandParams(std::vector<label *> params, MacroDef * macroThis);

    std::unordered_map<std::string, MacroDef *> macroMap;
    int macroExpandParamsDoDaniel(MacroCall * macrocall, int k);

    void replaceAll(std::string& str, const std::string& from, const std::string& to);

public:
    Assembler(std::vector<Semantic *> * lines);

    std::string preproccess();

    void init(bool willExecute);
    int preproccessDoDaniel (std::vector<Semantic *> * lines, int k);



};

#endif // ASSEMBLER_H
