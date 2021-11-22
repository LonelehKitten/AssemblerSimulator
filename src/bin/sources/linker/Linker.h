#ifndef LINKER_H
#define LINKER_H

#include <string>
#include <vector>
#include "../compiler/LinkerProto.h"
#include "../assembler/Assembler.h"

class Linker : public LinkerProto
{

    protected:

        //virtual bool tableInstructions(Assembler * , bool &error) = 0;
        virtual bool tableInstructions(Assembler * assembler, bool &error) override;
        virtual bool generateBytecode(Assembler * assembler, bool &error) override;

        int startProgramAsInteger;


    public:
        Linker(std::vector<Assembler *> * assembledCodes, int programCounter, SymbolTable * globalSymbols);

        // retorna flag de erro (sucesso: true, erro: false)
        bool link();
        int getStartProgramAsInteger();
        
        virtual bool resolveDependencies(std::vector<LinkerProto::PendingResolution *> &dependenciesMap, bool strict, bool * symbolWasResolvedHolder = nullptr) override;
};


#endif // LINKER_H
