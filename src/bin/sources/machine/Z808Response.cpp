#include "Z808Response.h"

void Z808Response::setAx(USint newAx)
{
    ax = newAx;
}

void Z808Response::setDx(USint newDx)
{
    dx = newDx;
}

void Z808Response::setSi(USint newSi)
{
    si = newSi;
}

void Z808Response::setSs(USint newSs)
{
    ss = newSs;
}

void Z808Response::setDs(USint newDs)
{
    ds = newDs;
}

void Z808Response::setCs(USint newCs)
{
    cs = newCs;
}

void Z808Response::setSp(USint newSp)
{
    sp = newSp;
}

void Z808Response::setPc(USint newPc)
{
    pc = newPc;
}

void Z808Response::setSr(std::bitset<16> * sr)
{
    this->sr = sr;
}

void Z808Response::setStandardOutput(const std::string newStdout)
{
    standardOutput = newStdout;
}

void Z808Response::setStandardInput(bool newStdin)
{
    standardInput = newStdin;
}

void Z808Response::setMemoryWrite(std::pair<USint, USint> * memoryWrite)
{
    this->memoryWrite = memoryWrite;
}

Z808Response::Z808Response() :
    ax(0),
    dx(0),
    si(0),
    ss(0),
    ds(0),
    cs(0),
    sp(0),
    pc(0),
    sr(nullptr),
    standardOutput(""),
    standardInput(false),
    memoryWrite(nullptr)
{}

std::string Z808Response::i(USint n) {
    return std::to_string(n);
}

std::string Z808Response::bits() {
    std::string bits = "[ ";
    for(int i = 0; i < (int) sr->size(); i++) {
        bits += this->i((*sr)[i]);
        if(i+1 < (int) sr->size()) bits += ", ";
    }
    bits += "]";
    return bits;
}

JSON Z808Response::toJSON() {

    JSON json = "";
    std::cout << "response: 1" << std::endl;

    json += std::string("{\n");
    json += std::string("    \"registers\": {\n");
    std::cout << "response: 2" << std::endl;
    json += std::string("        \"AX\": ") + i(ax) + std::string(",\n");
    json += std::string("        \"DX\": ") + i(dx) + std::string(",\n");
    json += std::string("        \"SI\": ") + i(si) + std::string(",\n");
    json += std::string("        \"SS\": ") + i(ss) + std::string(",\n");
    json += std::string("        \"DS\": ") + i(ds) + std::string(",\n");
    json += std::string("        \"CS\": ") + i(cs) + std::string(",\n");
    json += std::string("        \"SP\": ") + i(sp) + std::string(",\n");
    json += std::string("        \"PC\": ") + i(pc) + std::string(",\n");
    std::cout << "response: 3" << std::endl;
    json += std::string("        \"SR\": {\n");
    json += std::string("            \"asLiteral\": ") + i((USint) sr->to_ulong()) + std::string(",\n");
    std::cout << "response: 4" << std::endl;
    json += std::string("            \"asFlags\": ") + bits() + std::string("\n");
    std::cout << "response: 5" << std::endl;
    json += std::string("        }\n");
    json += std::string("    },\n");
    json += std::string("    \"stdout\": \"") + standardOutput + std::string("\",\n");
    json += std::string("    \"stdin\": ") + std::string(standardInput ? "true" : "false") + std::string(",\n");
    json += std::string("    \"memoryChange\": {\n");
    std::cout << "response: 6" << std::endl;
    if(memoryWrite != nullptr) {
        json += std::string("        \"address\": ") + i(memoryWrite->first) + std::string(", \n");
        json += std::string("        \"newValue\": ") + i(memoryWrite->second) + std::string(" \n");
        delete memoryWrite;
        memoryWrite = nullptr;
    }
    std::cout << "response: 7" << std::endl;
    json += std::string("    }\n");
    json += std::string("}");

    standardOutput = "";
    standardInput = false;

    return json;
}
