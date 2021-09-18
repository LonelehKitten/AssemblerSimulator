#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <vector>
#include <iostream>
#include <string>
#include "../analyzer/Semantic.h"
#include "macro.h"

class Assembler
{

private:
    std::vector<Semantic *> * lines;
    std::vector<macroDef *> macroTable;
    std::vector<label *> labelTable;

public:
    Assembler(std::vector<Semantic *> * lines);

    std::string preproccess();

};

#endif // ASSEMBLER_H
