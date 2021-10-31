#ifndef SYMBOL_H
#define SYMBOL_H
#include <string>
#include <vector>

struct Symbol
{
    std::string name;
    std::string value;
    std::vector<int> addressOccurrences;

    Symbol(std::string, std::string, int);
    Symbol(std::string, std::string);
};

#endif /* SYMBOL_H */
