#ifndef LINKER_H
#define LINKER_H

#include <string>
#include <vector>

#include "../compiler/Compiler.h"
#include "../assembler/Assembler.h"

class Linker : public Compiler
{
    private:

        std::vector<Assembler *> * assembledCodes;

    protected:



        virtual bool tableInstructions(Semantic * line, bool &error) override;
        virtual bool generateBytecode(Semantic * line, bool &error) override;

    public:
        Linker(std::vector<Assembler *> * assembledCodes);

        // retorna flag de erro (sucesso: true, erro: false)
        bool link();
};

#endif // LINKER_H
