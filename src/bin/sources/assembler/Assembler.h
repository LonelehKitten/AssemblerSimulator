#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <unordered_map>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include "macroDef.h"
#include "../analyzer/Semantic.h"
#include "../analyzer/RecognitionManager.h"

class Assembler
{

private:
    std::vector<Semantic *> * lines;
    std::vector<macroDef *> macroTable;
    std::vector<label *> labelTable;

    std::string macroExpandParams(std::vector<label *> params, macroDef * macroThis);

    std::unordered_map<std::string, macroDef *> macroMap;
    int macroExpandParamsDoDaniel(MacroCall * macrocall, std::string& output, int k);

public:
    Assembler(std::vector<Semantic *> * lines);

    std::string preproccess();

    std::string init();
    int preproccessDoDaniel (std::vector<Semantic *> * lines, std::string& output, int k);

};

#endif // ASSEMBLER_H
