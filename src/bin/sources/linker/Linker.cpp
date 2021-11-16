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
    // iterar entre os segmentos
    for(auto it_segment = segmentTable->begin(); it_segment != segmentTable->end(); it_segment++){
        // iterar os simbolos do segmento atual
        for(auto it_symbol = it_segment->second->getSymbol()->begin(); it_symbol != it_segment->second->getSymbol()->end(); it_symbol++){
            //it_symbol->second == objeto da classe Symbol
            tableGlobalSymbol.insert({it_symbol->second->name, new ExternalSymbol(it_symbol->second, std::stoi(it_symbol->second->value), 0) });
        }
    }

    // 1ª tabela copiada sem alterações, na 2ª tabela o valor dos endereços é adicionado do tamanho do primeiro segmento
    // para levar em conta o deslocamento do espaço ocupado pelo primeiro segmento
}

void Linker::linkerStep2()
{
    
    for (auto it_linker = tableGlobalSymbol.begin(); it_linker != tableGlobalSymbol.end(); it_linker++){

    }

    
    // Carrega o texto, relocando os símbolos locais relativos(não precisa de reajuste nas tabelas, coloca diretamente a informação)
    // Ajusta e reloca os símbolos definidos externamente
}