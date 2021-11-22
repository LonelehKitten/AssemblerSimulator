#include "LinkerProto.h"
#include "../GlobalSettings.h"
#ifdef PRODUCTION_MODE
#include "../InterfaceBus.h"
#endif


LinkerProto::PendingResolution::PendingResolution(Symbol *symbol, SegmentDef *segment, Equ *semantic) :
    symbol(symbol), segment(segment), semantic(semantic)
{}


LinkerProto::LinkerProto(std::vector<Assembler *> *file, int programCounter, SymbolTable * globalSymbols) :
    file(file),
    currentSegment(nullptr),
    assumedProgramSegment(nullptr),
    assumedDataSegment(nullptr),
    assumedStackSegment(nullptr),
    lineCounter(0),
    programCounter(programCounter),
    segmentCounter(0),
    startProgram(nullptr),
    inSegment(false),
    error(false),
    globalSymbols(globalSymbols)
{
}

// protected

bool LinkerProto::resolveDependencies(std::vector<PendingResolution *> &dependenciesMap, bool strict, bool * symbolWasResolvedHolder) {

    if(dependenciesMap.empty()) return true;

    PendingResolution * dep;
    bool symbolWasResolved = false;
    USint value;
    do
    {
        for (auto i = dependenciesMap.begin(); i != dependenciesMap.end(); i++)
        {
            dep = *i;
            if (evaluate(dep->semantic->getExpression(), &value) != nullptr)
            {
                dep->symbol->value = std::to_string(value);
                delete dep;
                dependenciesMap.erase(i);
                symbolWasResolved = true;
                i--;
            }
        }

        if(symbolWasResolvedHolder != nullptr)
                *symbolWasResolvedHolder = symbolWasResolved;

        if (!symbolWasResolved)
        {
            /*
            Error: Quando nao consegue resolver nenhum
            Só que tem um porém: É natural q ele não consegue resolver
            todos caso um simbolo externo, não esteja resolvido
            Mas isso não é verdade para simbolo que não são externos.
            Só retorna true se caso um dos simbolos do dependenciesMap esteja contido no, pendingExternalSymbols
            Se o nome da dependencia estiver contido, pelo menos um pode retornar true.

            iterar no dependenciesMap e ver se algum tem no pendingExternalSymbols
            */

            // um dos simbolos do dependenciesMap esteja contido no, pendingExternalSymbols
            for (auto i = dependenciesMap.begin(); !strict && i != dependenciesMap.end(); i++)
            {
                dep = *i;
                //Se o nome da dependencia estiver contido, pelo menos um pode retornar true.
                if(pendingExternalSymbols.find(dep->symbol->name) != pendingExternalSymbols.end()){ // ?
                    return true;
                }
            }

            //mensagem de erro no console, jogar oq tem no dependencyMap
            LOG(std::string("Error: Cannot resolve symbol"));
            this->error = true;
            return false;
        }
        symbolWasResolved = false;
    } while ((int)dependenciesMap.size() > 0);

    return true;
}


bool LinkerProto::stepTableInstructions(bool isBasic) {

    error = true;

    programCounter = 0;
    segmentCounter = 0;

    inSegment = false;

    Assembler *  line;

    for (int i = 0; i < (int) file->size(); ++i)
    {
        line = file->at(i);

        if(tableInstructions(line, error)) {
            if(resolveDependencies(dependenciesMap, isBasic)) {

                for(auto pSymbol : publicSymbols)
                    globalSymbols->insert(std::make_pair(pSymbol.first, pSymbol.second));

                return true;
            }
        }

    }

    return false;

}


bool LinkerProto::stepGenerateBytecode() {

    error = false;

    startProgram = 0;
    programCounter = 0;
    segmentCounter = 0;

    inSegment = false;

    Assembler * line;

    for (int i = 0; !error && i < (int) file->size(); ++i)
    {
        line = file->at(i);

        if(generateBytecode(line, error)) { // é a etapa 2
            return true;
        }

    }

    return false;

}


std::vector<byte> * LinkerProto::getBytecode() {
    return &bytecode;
}


bool LinkerProto::containsStartProgram() {
    return startProgram != nullptr;
}


SegmentDef * LinkerProto::getAssumedDataSegment(){
    return assumedDataSegment;
}


SegmentDef * LinkerProto::getCurrentSegment(){
    return currentSegment;
}


SegmentDef * LinkerProto::getAssumedProgramSegment(){
    return assumedProgramSegment;
}


SegmentDef * LinkerProto::getAssumedStackSegment() {
    return assumedStackSegment;
}


SymbolTable * LinkerProto::getSegmentTable(){
    return &segmentTable;
}


bool LinkerProto::isError(){
    return error;
}


int LinkerProto::getLineCounter(){
    return lineCounter;
}


int LinkerProto::getProgramCounter(){
    return programCounter;
}


int LinkerProto::getSegmentCounter(){
    return segmentCounter;
}


int LinkerProto::getStartProgram(){
    if(startProgram == nullptr) return 0;
    return std::stoi(currentSegment->value) + std::stoi(startProgram->value);
}


SymbolTable * LinkerProto::getPublicSymbols(){
    return &publicSymbols;
}


SymbolTable * LinkerProto::getModules(){
    return &modules;
}


SymbolTable * LinkerProto::getGlobalSymbols(){
    return globalSymbols;
}


std::vector<LinkerProto::PendingResolution *> * LinkerProto::getDependenciesMap(){
    return &dependenciesMap;
}
