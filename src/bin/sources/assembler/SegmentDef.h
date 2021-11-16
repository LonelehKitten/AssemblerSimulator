#ifndef SEGMENT_DEF_H
#define SEGMENT_DEF_H

#include "../Enums.h"

class SegmentDef
{
private:
    std::string name;
    int location;
    int size;
    
    SymbolTable symbolTable;
public:
    SegmentDef(std::string, int, int);

    int getSize();

    std::string getName();

    int getLocation();

    void setSymbol(Symbol *);

    Symbol * getSymbol(std::string);

    SymbolTable * getSymbol();
};

#endif /* SEGMENT_DEF_H */
