#include "SegmentDef.h"

SegmentDef::SegmentDef(std::string name, int location, int size)
{
    this->name = name;
    this->location = location;
    this->size = size;
}

std::string SegmentDef::getName()
{
    return name;
}

int SegmentDef::getLocation()
{
    return location;
}

void SegmentDef::setSize(int size)
{
    this->size = size;
}

int SegmentDef::getSize()
{
    return size;
}


void SegmentDef::setSymbol(Symbol * symbol)
{
    if (symbol != nullptr)
        symbolTable.insert({symbol->name,symbol});
}

Symbol * SegmentDef::getSymbol(std::string symbolName)
{
    if (symbolTable.find(symbolName) == symbolTable.end())
        return nullptr;
    return symbolTable.find(symbolName)->second;
}

SymbolTable * SegmentDef::getSymbol()
{
    return &symbolTable;
}