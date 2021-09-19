#ifndef ASSEMBLER_H
#define ASSEMBLER_H

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

public:
    Assembler(std::vector<Semantic *> * lines);

    std::string preproccess();

};

#endif // ASSEMBLER_H
