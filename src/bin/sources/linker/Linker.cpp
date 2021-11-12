#include "Linker.h"

Linker::Linker(SegmentMap * segmentTable, ExternalSymbolMap * tableDefinition, ExternalUseMap *tableUse)
{
    this->segmentTable = segmentTable;
    this->tableDefinition = tableDefinition;
    this->tableUse = tableUse;

    //this->tableGlobalSymbol = uniao_das_tabelas_de_simbolos_dos_segmentos;
}

void Linker::linkerStep1()
{

}

void Linker::linkerStep2()
{

}