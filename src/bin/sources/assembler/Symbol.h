#ifndef SYMBOL_H
#define SYMBOL_H
#include <string>

struct Symbol
{
    std::string name;
    std::string value;
    int address;
    
    Symbol(std::string, std::string, int);
};

#endif /* SYMBOL_H */
