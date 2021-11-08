#include "Symbol.h"

Symbol::Symbol(std::string name, std::string value, bool isVar, bool isLabel) :
    name(name), 
    value(value), 
    isVar(isVar),
    isLabel(isLabel)
{}

Symbol::Symbol(std::string name, std::string value) :
    name(name), 
    value(value),
    isVar(false),
    isLabel(false)
{}
