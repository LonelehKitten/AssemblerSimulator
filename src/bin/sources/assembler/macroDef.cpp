#include "MacroDef.h"

MacroDef::MacroDef(std::string name, std::vector<std::string> args)
{
    this->name = name;
    this->args = args;
    this->macroText = new std::vector<Semantic *>();
}

std::string MacroDef::getName()
{
    return this->name;
}

std::vector<std::string> MacroDef::getArgs()
{
    return this->args;
}

std::vector<Semantic *> * MacroDef::getText()
{
    return this->macroText;
}

void MacroDef::setText(Semantic * line)
{
    this->macroText->emplace_back(line);
}


std::string MacroDef::macroLine()
{
}

void MacroDef::updateMacro(std::vector<std::string> args)
{
}
