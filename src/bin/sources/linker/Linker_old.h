#ifndef LINKER_OLD_H
#define LINKER_OLD_H

#include <unordered_map>
#include <vector>
#include <string>

#include "../compiler/SegmentDef.h"
#include "LinkerTables_old.h"

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

#endif // LINKER_OLD_H
