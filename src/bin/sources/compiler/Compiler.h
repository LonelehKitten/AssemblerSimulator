#ifndef COMPILER_H
#define COMPILER_H

#include <vector>
#include <string>
#include "Symbol.h"
#include "SegmentDef.h"
#include "ExpressionEvaluator.h"
#include "../analyzer/Semantic.h"
#include "../Enums.h"
#include "../GlobalSettings.h"


class Compiler {
    protected:
        struct PendingResolution {
            Symbol *symbol;
            SegmentDef *segment;
            Equ *semantic;
            PendingResolution(Symbol *symbol, SegmentDef *segment, Equ *semantic);
        };

        std::vector<Semantic *> * file;

        std::vector<byte> bytecode;
        std::vector<PendingResolution *> dependenciesMap;

        SymbolTable segmentTable;

        SegmentDef * currentSegment;
        SegmentDef * assumedProgramSegment, * assumedDataSegment, * assumedStackSegment;

        // contadores
        int lineCounter;
        int programCounter;
        int segmentCounter;

        Symbol * startProgram;

        // flags
        bool inSegment;
        bool error;

        // metodos
        bool resolveDependencies(std::vector<PendingResolution *> &dependenciesMap);
        
        bool stepTableInstructions();
        bool stepGenerateBytecode();

        // metodos abstratos
        virtual bool tableInstructions(Semantic * line, bool &error) = 0;
        virtual bool generateBytecode(Semantic * line, bool &error) = 0;

        //construtor
        Compiler(std::vector<Semantic *> *file);

        // usado no modo completo (com linker)
        SymbolTable publicSymbols;
        SymbolTable externalSymbols;
        SymbolTable modules;


    public:
        std::vector<byte> *evaluate(Expression *expression, USint *valueHolder, bool * isConst = nullptr);

        std::vector<byte> * getBytecode();
        int getStartProgram();
        bool containsStartProgram();
        
};


#endif /* COMPILER_H */
