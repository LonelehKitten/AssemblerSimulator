#include "AssemblerProto.h"
#include "../GlobalSettings.h"
#ifdef PRODUCTION_MODE
#include "../InterfaceBus.h"
#endif

AssemblerProto::PendingResolution::PendingResolution(Symbol *symbol, SegmentDef *segment, Equ *semantic) :
    symbol(symbol), segment(segment), semantic(semantic)
{}

AssemblerProto::AssemblerProto(std::vector<Semantic *> *file, int programCounter, SymbolTable * globalSymbols) :
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
bool AssemblerProto::resolveDependencies(std::vector<PendingResolution *> &dependenciesMap, bool strict, bool * symbolWasResolvedHolder) {

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

bool AssemblerProto::stepTableInstructions(bool isBasic) {

    error = true;

    programCounter = 0;
    segmentCounter = 0;

    inSegment = false;

    Semantic * line;

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

bool AssemblerProto::stepGenerateBytecode() {

    error = false;

    startProgram = 0;
    programCounter = 0;
    segmentCounter = 0;

    inSegment = false;

    Semantic * line;

    for (int i = 0; !error && i < (int) file->size(); ++i)
    {
        line = file->at(i);

        if(generateBytecode(line, error)) { // é a etapa 2
            return true;
        }

    }

    return false;

}

std::vector<byte> * AssemblerProto::getBytecode() {
    return &bytecode;
}

bool AssemblerProto::containsStartProgram() {
    return startProgram != nullptr;
}

SegmentDef * AssemblerProto::getAssumedDataSegment(){
    return assumedDataSegment;
}

SegmentDef * AssemblerProto::getCurrentSegment(){
    return currentSegment;
}

SegmentDef * AssemblerProto::getAssumedProgramSegment(){
    return assumedProgramSegment;
}

SegmentDef * AssemblerProto::getAssumedStackSegment() {
    return assumedStackSegment;
}

SymbolTable * AssemblerProto::getSegmentTable(){
    return &segmentTable;
}

bool AssemblerProto::isError(){
    return error;
}

int AssemblerProto::getLineCounter(){
    return lineCounter;
}

int AssemblerProto::getProgramCounter(){
    return programCounter;
}

int AssemblerProto::getSegmentCounter(){
    return segmentCounter;
}

int AssemblerProto::getStartProgram(){
    if(startProgram == nullptr) return 0;
    return std::stoi(currentSegment->value) + std::stoi(startProgram->value);
}

SymbolTable * AssemblerProto::getPublicSymbols(){
    return &publicSymbols;
}

SymbolTable * AssemblerProto::getModules(){
    return &modules;
}

SymbolTable * AssemblerProto::getGlobalSymbols(){
    return globalSymbols;
}

std::vector<AssemblerProto::PendingResolution *> * AssemblerProto::getDependenciesMap(){
    return &dependenciesMap;
}
