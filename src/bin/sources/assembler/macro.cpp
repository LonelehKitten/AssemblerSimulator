#include "template.h"

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

std::string macroDef::getText()
{
    return this->macroText;
}

void macroDef::setText(std::string line)
{
    this->macroText = this->macroText + line + '\n';
}


std::string macroDef::macroLine()
{
}

void macroDef::updateMacro(std::vector<std::string> args)
{
}