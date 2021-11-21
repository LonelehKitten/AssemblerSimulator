#ifndef SEGMENT_DEF_H
#define SEGMENT_DEF_H

#include "../Enums.h"

class SegmentDef : public Symbol
{
private:
    USint location;
    USint size;
    
    SymbolTable symbolTable;
public:
    SegmentDef(std::string, USint, USint);
    
    void setSize(USint);

    USint getSize();

    void setLocation(USint location);

    USint getLocation();

    void setSymbol(Symbol *);

    Symbol * getSymbol(std::string);

    SymbolTable * getSymbol();
};

#endif /* SEGMENT_DEF_H */
