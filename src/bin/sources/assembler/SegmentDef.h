#ifndef SEGMENT_DEF_H
#define SEGMENT_DEF_H

#include "../Enums.h"

class SegmentDef
{
private:
    std::string name;
    int location;
    
    SymbolTable symbolTable;
public:
    SegmentDef(std::string, int);

    std::string getName();

    int getLocation();

    void setSymbol(Symbol *);

    Symbol * getSymbol(std::string);
};

#endif /* SEGMENT_DEF_H */
