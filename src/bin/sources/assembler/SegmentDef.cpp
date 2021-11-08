#include "SegmentDef.h"

SegmentDef::SegmentDef(std::string name, int location)
{
    this->name = name;
    this->location = location;
}

std::string SegmentDef::getName()
{
    return name;
}

int SegmentDef::getLocation()
{
    return location;
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
