#include "Semantic.h"

Semantic::Semantic(std::string line)
    : line(line) {}

Semantic::Semantic(std::string line, Instruction type)
    : line(line), type(type) {}

Semantic::Semantic(std::string line, std::string name, Instruction type)
    : line(line), name(name), type(type) {}

Semantic::Semantic(std::string line, std::string name, Instruction type, std::vector<std::string> * params)
    : line(line), name(name), type(type), params(params) {}

std::string Semantic::getLine() {
    return line;
}

Instruction Semantic::getType() {
    return type;
}
