#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <unordered_map>
#include <vector>
#include <iostream>
#include <string>
#include "MacroDef.h"
#include "../compiler/Symbol.h"
#include "../compiler/ExpressionEvaluator.h"
#include "../compiler/SegmentDef.h"
#include "../Utils.h"
#include "../compiler/AssemblerProto.h"

class Assembler : public AssemblerProto
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
    template <class T> void tableVarInstruction(T *t);

protected:

    virtual bool tableInstructions(Semantic * line, bool &error) override;
    virtual bool generateBytecode(Semantic * line, bool &error) override;

public:

    Assembler(std::vector<Semantic *> * lines, int programCounter, SymbolTable * globalSymbols);

    //Método principal para chamar o montador (PRÉPROCESSADOR PRECISA SER CHAMADO ANTES)
    //Recebe como parâmetro o modo de montagem (basico: true, completo: false)
    //Retorna true se montado com sucesso
    bool assemble(bool assemblerType);

    long getStartSegment();

    void init(bool willExecute);
    int preproccess (std::vector<Semantic *> * lines, int k);

    std::vector<byte> *evaluate(Expression *expression, USint *valueHolder, bool * isConst = nullptr);


    // debug only
    std::string getOutput();

};

#endif // ASSEMBLER_H
