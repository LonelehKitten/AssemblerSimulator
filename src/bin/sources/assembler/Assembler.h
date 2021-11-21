#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <unordered_map>
#include <vector>
#include <iostream>
#include <string>
#include "MacroDef.h"
#include "Symbol.h"
#include "ExpressionEvaluator.h"
#include "SegmentDef.h"
#include "../Utils.h"
#include "../compiler/Compiler.h"

class Assembler : public Compiler
{
private:

    std::vector<MacroDef *> macroList;
    std::unordered_map<std::string, MacroDef *> macroTable;

    std::string output;

    template <class T> std::vector<byte> * generateAssembly(T * line);
    template <class T> std::vector<byte> * generateAssemblyJumps(T *line);
    void GetSpecialOpcode(Semantic * line);

    int macroExpandParams(MacroCall * macrocall, int k);
    
    template <class T> void tableArithmeticInstructions(T *t);
    template <class T> void tableJumpsInstruction(T *t);
    template <class T> void tableIntInstruction(T *t);
    template <class T> void tableVarInstruction(T *t, bool isConst);

    virtual bool tableInstructions(Semantic * line, bool &error);
    virtual bool generateBytecode(Semantic * line, bool &error);

public:

    Assembler(std::vector<Semantic *> * lines);

    //Método principal para chamar o montador (PRÉPROCESSADOR PRECISA SER CHAMADO ANTES)
    //Recebe como parâmetro o modo de montagem (basico: true, completo: false)
    //Retorna true se montado com sucesso
    bool assemble(bool assemblerType);

    long getStartProgram();
    long getStartSegment();

    void init(bool willExecute);
    int preproccess (std::vector<Semantic *> * lines, int k);

    std::vector<byte> * getAssemblyCode();

    // debug only
    std::string getOutput();

};

#endif // ASSEMBLER_H
