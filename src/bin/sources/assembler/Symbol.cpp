#include "Symbol.h"

Symbol::Symbol(std::string name, std::string value, int address)
{
    this->name = name;
    this->value = value;
    this->addressOccurrences.push_back(address);
}

Symbol::Symbol(std::string name, std::string value)
{
    this->name = name;
    this->value = value;
}
