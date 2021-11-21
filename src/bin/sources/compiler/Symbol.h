#ifndef SYMBOL_H
#define SYMBOL_H
#include <string>
#include <vector>

struct Symbol
{
    std::string name;
    std::string value;
    bool isVar, isLabel;
    std::vector<int> addressOccurrences;

    Symbol(std::string, std::string, bool, bool);
    Symbol(std::string, std::string);
};

#endif /* SYMBOL_H */
