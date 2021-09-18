#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <vector>
#include <iostream>
#include <string>
#include "../analyzer/Semantic.h"
<<<<<<< HEAD
#include "macroDef.h"
=======
#include "template.h"
>>>>>>> 19d9fba70ef35e0a348157f8da31e081225bb791

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
