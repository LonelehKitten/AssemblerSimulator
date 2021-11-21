#ifndef COMPILER_H
#define COMPILER_H

#include <vector>
#include <string>
#include "Symbol.h"
#include "SegmentDef.h"
#include "ExpressionEvaluator.h"
#include "../analyzer/Semantic.h"
#include "../Enums.h"

class Compiler {
    private:
        std::vector<Semantic *> * file;
        bool error;

    protected:
        struct PendingResolution {
            Symbol *symbol;
            SegmentDef *segment;
            Equ *semantic;
            PendingResolution(Symbol *symbol, SegmentDef *segment, Equ *semantic);
        };

        std::vector<byte> bytecode;

        SymbolTable segmentTable;

        SegmentDef * currentSegment;
        SegmentDef * assumedProgramSegment, * assumedDataSegment, * assumedStackSegment;

        // contadores
        int lineCounter;
        int programCounter;
        int segmentCounter;
        int startProgram;

        // flags
        bool inSegment;

        // metodos
        bool resolveDependencies(std::vector<PendingResolution *> &dependenciesMap);
        
        bool tableInstructions();
        bool generateBytecode();

        // metodos abstratos
        virtual bool tableInstructions(Semantic * line, bool &error) = 0;
        virtual bool generateBytecode(Semantic * line, bool &error) = 0;
    public:
        std::vector<byte> *evaluate(Expression *expression, USint *valueHolder, bool * isConst);
        
};


#endif /* COMPILER_H */