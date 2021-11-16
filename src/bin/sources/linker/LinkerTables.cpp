#include "LinkerTables.h"

ExternalSymbol::ExternalSymbol(Symbol* symbol, int address, int reloc)
{
    this->symbol = symbol;
    this->address = address;
    this->reloc = reloc;
}

ExternalUse::ExternalUse(Symbol* symbol, int LC, int reloc, int opSignal)
{
    this->symbol = symbol;
    this->LC = LC;
    this->reloc = reloc;
    this->opSignal = opSignal;
}