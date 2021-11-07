#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include "analyzer/Token.h"
#include "assembler/Symbol.h"

typedef unsigned short int USint;
typedef std::string JSON;
typedef unsigned char byte;
typedef std::vector<Token *> Expression;
typedef std::unordered_map<std::string, Symbol *> SymbolTable;