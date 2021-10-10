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

void Z808Response::setStdout(const std::string newStdout)
{
    stdout = newStdout;
}

void Z808Response::setStdin(bool newStdin)
{
    stdin = newStdin;
}

void Z808Response::setMemoryWrite(std::pair<USint, USint> * memoryWrite)
{
    this->memoryWrite = memoryWrite;
}

Z808Response::Z808Response() {

}

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

    std::string JSON = "";

    JSON += std::string();
    JSON += std::string("{\n");
    JSON += std::string("    \"registers\": {\n");
    JSON += std::string("        \"AX\": ") + i(ax) + std::string(",\n");
    JSON += std::string("        \"DX\": ") + i(dx) + std::string(",\n");
    JSON += std::string("        \"SI\": ") + i(si) + std::string(",\n");
    JSON += std::string("        \"SS\": ") + i(ss) + std::string(",\n");
    JSON += std::string("        \"DS\": ") + i(ds) + std::string(",\n");
    JSON += std::string("        \"CS\": ") + i(cs) + std::string(",\n");
    JSON += std::string("        \"SP\": ") + i(sp) + std::string(",\n");
    JSON += std::string("        \"PC\": ") + i(pc) + std::string(",\n");
    JSON += std::string("        \"SR\": {\n") +
    JSON += std::string("            \"asLiteral\": ") + i((USint) sr->to_ulong()) + std::string(",\n");
    JSON += std::string("            \"asFlags\": ") + bits() + std::string(",\n");
    JSON += std::string("        }\n") +
    JSON += std::string("    },\n") +
    JSON += std::string("    \"stdout\": \"") + stdout + std::string("\",\n");
    JSON += std::string("    \"stdin\": ") + (stdin ? "true" : "false") + std::string(",\n");
    JSON += std::string("    \"memoryChange\": {\n");
    if(memoryWrite == nullptr) {
        JSON += std::string("        \"address\": ") + i(memoryWrite->first) + std::string(", \n");
        JSON += std::string("        \"newValue\": ") + i(memoryWrite->second) + std::string(", \n");
    }
    JSON += std::string("    }\n");
    JSON += std::string("}");

    stdout = "";
    stdin = false;
    delete memoryWrite;
    memoryWrite = nullptr;

    return JSON;
}
