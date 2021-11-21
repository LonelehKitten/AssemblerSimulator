#ifndef LINKER_TABLES_H
#define LINKER_TABLES_H

#include "../compiler/Symbol.h"

struct ExternalSymbol{
    //- Símbolo
    Symbol * symbol;
    //- Endereço
    int address;
    //- Modo de relocabilidade (relativo ou absoluto) 
    int reloc;
    
    ExternalSymbol(Symbol*, int, int);
};

//linker/LinkerTables.cpp:11:67: error: no 'int ExternalUse::ExternalSymbol(Symbol*, int, int)' member function declared in class 'ExternalUse'
//ExternalUse::ExternalSymbol(Symbol* symbol, int address, int reloc)

struct ExternalUse{
    //- Símbolo
	Symbol * symbol;
    //- Location counter (Endereço relativo) do campo de operandos
	int LC;
    //- Modo de relocabilidade (relativo ou absoluto)
	int reloc;
    //- Sinal da operação (permitir soma de endereços)
    int opSignal;

    ExternalUse(Symbol*, int, int, int);
};

#endif // LINKER_TABLES_H
