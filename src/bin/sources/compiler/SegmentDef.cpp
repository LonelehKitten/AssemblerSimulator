#include "SegmentDef.h"

SegmentDef::SegmentDef(std::string name, USint location, USint size) : Symbol::Symbol(name, std::to_string(location))
{
    //../src/bin/sources/assembler/SegmentDef.cpp:3:68: error: no matching function for call to ‘Symbol::Symbol()’
    /*
    this->name = name;
    this->value = location;
    this->isVar = false;
    this->isLabel = false;
    */
    this->location = location;
    this->size = size;
}

void SegmentDef::setLocation(USint location){
    this->location = location;
    value = std::to_string(location);
}

USint SegmentDef::getLocation()
{
    return location;
}

void SegmentDef::setSize(USint size)
{
    this->size = size;
}

USint SegmentDef::getSize()
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