#include "Compiler.h"

Compiler::PendingResolution::PendingResolution(Symbol *symbol, SegmentDef *segment, Equ *semantic) :
    symbol(symbol), segment(segment), semantic(semantic)
{}

// protected
bool Compiler::resolveDependencies(std::vector<PendingResolution *> &dependenciesMap) {
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
        if (!symbolWasResolved)
        {
            //mensagem de erro no console, jogar oq tem no dependencyMap
            LOG(std::string("Error: Cannot resolve symbol"));
            this->error = true;
            return false;
        }
        symbolWasResolved = false;
    } while ((int)dependenciesMap.size() > 0);

    return true;
}

/*
    lida com o ExpressionEvaluator e retorna o vetor de bytes do valor resultante
*/
std::vector<byte> *Compiler::evaluate(Expression *expression, USint *valueHolder, bool * isConst)
{
    ExpressionEvaluator *evaluator = 
        new ExpressionEvaluator(expression, assumedProgramSegment, assumedDataSegment, &segmentTable);
    if (evaluator->couldNotSymbolBeResolved())
    {
        delete evaluator;
        return nullptr;
    }

    USint value = evaluator->getValue();
    if (valueHolder != nullptr)
        *valueHolder = value;

    if(isConst != nullptr)
        *isConst = evaluator->isConst();

    delete evaluator;

    return new std::vector<byte>({(byte)(value & 0xFF), (byte)(value >> 8)});
}

bool Compiler::tableInstructions() {

    error = true;

    programCounter = 0;
    segmentCounter = 0;

    inSegment = false;

    Semantic * line;

    for (int i = 0; i < file->size(); ++i)
    {
        line = file->at(i);

        if(tableInstructions(line, error)) {
            return resolveDependencies(dependenciesMap);
        }

    }

    return false;

}

bool Compiler::generateBytecode() {

    error = false;

    startProgram = 0;
    programCounter = 0;
    segmentCounter = 0;

    inSegment = false;

    Semantic * line;

    for (int i = 0; !error && i < file->size(); ++i)
    {
        line = file->at(i);

        if(generateBytecode(line, error)) {
            return true;
        }

    }

    return false;

}