#ifndef OBJECTFILEGENERATOR_H
#define OBJECTFILEGENERATOR_H

#include <unordered_map>
#include <vector>
#include <iostream>
#include <string>
#include "MacroDef.h"
#include "Symbol.h"
#include "ExpressionEvaluator.h"
#include "SegmentDef.h"
#include "../analyzer/Dictionaries.h"
#include "../Utils.h"

class ObjectFileGenerator{
private:

    template<class T> void ObjectFileGenerator::JumpsIterator(InstructionObject type, std::vector<byte> * file, Semantic * line);
    void Generate();
    void ObjectFileGenerator::exportExpression(std::vector<Token *> * expression, std::vector<byte> * file);
    ObjectFileGenerator::exportString(std::string str, std::vector<byte> * file)

public:

    ObjectFileGenerator(std::vector<Semantic *> * lines);
}

#endif // OBJECTFILEGENERATOR_H