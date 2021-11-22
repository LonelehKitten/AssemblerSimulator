#include "Linker.h"

Linker::Linker(std::vector<Assembler *> * assembledCodes, int programCounter, SymbolTable * globalSymbols) :
    LinkerProto(assembledCodes, programCounter, globalSymbols)
{
}

bool Linker::resolveDependencies(std::vector<LinkerProto::PendingResolution *> &dependenciesMap,
    bool strict, bool * symbolWasResolvedHolder) {

    std::vector<Assembler *> pendingAssemblers(*file);

    bool symbolWasResolved = false;

    Assembler * a;

    do {

        for(int i = 0; i < (int) pendingAssemblers.size(); i++) {

            a = pendingAssemblers[i]; 

            if(a->resolveDependencies(*(a->getDependenciesMap()), false)) {

                symbolWasResolved = true;

                if(a->getDependenciesMap()->empty()) {
                    pendingAssemblers.erase(pendingAssemblers.begin()+i);
                    i--;
                }

            }

        }

        if(!symbolWasResolved) {
            return false;
        }

        symbolWasResolved = false;

    } while((int) pendingAssemblers.size() > 0);

    return true;
}
//virtual bool tableInstructions(Assembler * , bool &error) = 0;
bool Linker::tableInstructions(Assembler * assembler, bool &error) {

    if(assembler->containsStartProgram()) {
        error = false;
        return true;
    }
    
    return false;

}

bool Linker::generateBytecode(Assembler * assembler, bool &error) {
    //sendo executado para cada assembler
    //contruir o byte code do assembler
    //bool = true, fez todos
    //bool = error caso tenha dado erro durante a geração de byte code
    error = !assembler->stepGenerateBytecode();
    return false;
}

bool Linker::link() {

    if(stepTableInstructions(false)) {
        if(stepGenerateBytecode()) {

            for(auto it_assembler = file->begin(); it_assembler != file->end(); it_assembler++ ){

                std::vector<byte> * assembler_bytcode = (*it_assembler)->getBytecode(); 
                // Loop dos bytcodes
                for(auto it_bytecode = assembler_bytcode->begin(); it_bytecode != assembler_bytcode->end(); it_bytecode++){
                    bytecode.emplace_back(*it_bytecode);
                }
            }

            return true;

        }
    }

    return false;
}

int Linker::getStartProgramAsInteger() {
    return startProgramAsInteger;
}
