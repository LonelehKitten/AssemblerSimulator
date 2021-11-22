#ifndef COMPILER_H
#define COMPILER_H

#include <vector>
#include <string>
#include "Symbol.h"
#include "SegmentDef.h"
#include "ExpressionEvaluator.h"
#include "../analyzer/Semantic.h"
#include "../Enums.h"

class AssemblerProto {
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
        bool inSegment; // não precisa
        bool error;

        // metodos
        
        virtual bool stepTableInstructions(bool isBasic);

        // metodos abstratos
        virtual bool tableInstructions(Semantic * , bool &error) = 0;
        virtual bool generateBytecode(Semantic * , bool &error) = 0;

        //construtor
        AssemblerProto(std::vector<Semantic *> *file, int programCounter, SymbolTable * globalSymbols);

        // usado no modo completo (com linker)
        std::set<std::string> pendingPublicSymbols; // conjunto de nomes públicos
        std::unordered_map<std::string, std::string> pendingExternalSymbols; // mapa de símbolos externos (nome, tipo)
        
        SymbolTable publicSymbols;

        //será repasado para os demais assemblers
        SymbolTable * globalSymbols; // conjunto de todos os símbolos públicos;
        SymbolTable modules;

        std::string moduleName;
    public:
        virtual bool resolveDependencies(std::vector<PendingResolution *> &dependenciesMap, bool strict, bool * symbolWasResolvedHolder = nullptr);
        virtual std::vector<byte> *evaluate(Expression *expression, USint *valueHolder, bool * isConst = nullptr) {};
        std::vector<PendingResolution *> * getDependenciesMap();

        bool containsStartProgram();

        std::vector<Semantic *> * getFile();
        std::vector<byte> * getBytecode();

        SegmentDef * getAssumedDataSegment();
        SegmentDef * getCurrentSegment();
        SegmentDef * getAssumedProgramSegment();
        SegmentDef * getAssumedStackSegment();
        SymbolTable * getSegmentTable();

        int getLineCounter();
        int getProgramCounter();
        int getSegmentCounter();
        int getStartProgram();
        bool isError();

        SymbolTable * getPublicSymbols();
        SymbolTable * getModules();
        SymbolTable * getGlobalSymbols();

        virtual bool stepGenerateBytecode();

        /*
        void setStartProgram(int startProgram);
        void setLineCounter(int lineCounter);
        void setProgramCounter(int programCounter);
        void setSegmentCounter(int segmentCounter);
        */
        
};

#endif /* COMPILER_H */
