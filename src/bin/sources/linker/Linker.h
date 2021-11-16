#ifndef LINKER_H
#define LINKER_H

#include <unordered_map>
#include <vector>
#include <string>

#include "../assembler/SegmentDef.h"
#include "LinkerTables.h"

#include "../Utils.h"

class Linker
{
private:
    //Declaração das Tabelas
    typedef std::unordered_map<std::string, SegmentDef *> SegmentMap;
    typedef std::unordered_map<std::string, ExternalSymbol *> ExternalSymbolMap;
    typedef std::unordered_map<std::string, ExternalUse *> ExternalUseMap;

    ExternalSymbolMap tableGlobalSymbol;
    
    SegmentMap * segmentTable;
    ExternalSymbolMap * tableDefinition;
    ExternalUseMap * tableUse;
    
    //Os 2 passos do ligador
    void linkerStep1();
    void linkerStep2();

    std::vector<byte> memory;

public:
    Linker(SegmentMap * segmentTable, ExternalSymbolMap * tableDefinition, ExternalUseMap * tableUse);

    
};

#endif // LINKER_H