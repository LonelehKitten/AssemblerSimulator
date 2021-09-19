#include "macroDef.h"

macroDef::macroDef(std::string name, std::vector<std::string> args)
{
    this->name = name;
    this->args = args;
}

std::string macroDef::getName()
{
    return this->name;
}

std::vector<std::string> macroDef::getArgs()
{
    return this->args;
}

std::vector<Semantic *> macroDef::getText()
{
    return this->macroText;
}

void macroDef::setText(Semantic * line)
{
    this->macroText.push_back(line);
}


std::string macroDef::macroLine()
{
}

void macroDef::updateMacro(std::vector<std::string> args)
{
}