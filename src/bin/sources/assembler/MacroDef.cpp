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


std::vector<std::pair<std::string, std::string> *> * MacroDef::getParamValuePairs(MacroCall * macrocall) {

    std::vector<std::pair<std::string, std::string> *> * pairs = 
        new std::vector<std::pair<std::string, std::string> *>();
    std::string value;

    for(int i = 0; i < (int) args.size() && i < (int) macrocall->getParams()->size(); i++) {

        value = "";

        for (int j = 0; j < (int)macrocall->getParams()->at(i)->size(); j++) {

            value += macrocall->getParams()->at(i)->at(j)->getToken();

        }

        pairs->emplace_back(new std::pair<std::string, std::string>(Utils::toLowerCase(args[i]), Utils::toLowerCase(value)));
    }

    return pairs;

}