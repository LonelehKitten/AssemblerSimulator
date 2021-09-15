#include "Semantic.h"


Semantic::Semantic(std::string line, Instruction type)
    : line(line), type(type) {}
/*
Semantic::Semantic(std::string line)
    : line(line) {}

Semantic::Semantic(std::string line, std::string name, Instruction type)
    : line(line), name(name), type(type) {}

Semantic::Semantic(std::string line, std::string name, Instruction type, std::vector<std::string> * params)
    : line(line), name(name), type(type), params(params) {}
*/
std::string Semantic::getLine() {
    return line;
}

Instruction Semantic::getType() {
    return type;
}

//MACRO

Macro::Macro(std::string line, std::string name)
    : Semantic(line, Instruction::iMACRO), name(name) {}

Macro::Macro(std::string line, std::string name, std::vector<std::string> * params)
    : Semantic(line, Instruction::iMACRO), name(name), params(params) {}

std::string Macro::getName(){
    return name;
}

std::vector<std::string> * Macro::getParams() {
    return params;
}

EndM::EndM(std::string line) : Semantic(line, Instruction::iEND) {}

//instructions

Add::Add(std::string line, std::string segundoOperando)
    : Semantic(line, Instruction::iADD), segundoOperando(segundoOperando) {}
std::string Add::getSegundoOperando() {
    return segundoOperando;
}

Sub::Sub(std::string line, std::string segundoOperando)
    : Semantic(line, Instruction::iSUB), segundoOperando(segundoOperando) {}
std::string Sub::getSegundoOperando() {
    return segundoOperando;
}

Div::Div(std::string line, std::string operando)
    : Semantic(line, Instruction::iDIV), operando(operando) {}
std::string Div::getOperando() {
    return operando;
}

Mul::Mul(std::string line, std::string operando)
    : Semantic(line, Instruction::iMUL), operando(operando) {}
std::string Mul::getOperando() {
    return operando;
}

Cmp::Cmp(std::string line, std::string segundoOperando)
    : Semantic(line, Instruction::iCMP), segundoOperando(segundoOperando) {}
std::string Cmp::getSegundoOperando() {
    return segundoOperando;
}

//movimentação

Mov::Mov(std::string line, std::vector<std::string> * operandos)
    : Semantic(line, Instruction::iMOV), operandos(operandos) {}
std::vector<std::string> * Mov::getOperandos() {
    return operandos;
}

//pilha

Pop::Pop(std::string line)
    : Semantic(line, Instruction::iPOP) {}

Push::Push(std::string line)
    : Semantic(line, Instruction::iPUSH) {}

Poft::Poft(std::string line)
    : Semantic(line, Instruction::iPOPF) {}

Pushf::Pushf(std::string line)
    : Semantic(line, Instruction::iPUSHF) {}

//montagem

End::End(std::string line, std::string name)
    : Semantic(line, Instruction::iEND), name(name) {}
std::string End::getName() {
    return name;
}

Segment::Segment(std::string line, std::string name)
    : Semantic(line, Instruction::iSEGMENT), name(name) {}
std::string Segment::getName() {
    return name;
}

EndS::EndS(std::string line, std::string name)
    : Semantic(line, Instruction::iENDS), name(name) {}
std::string EndS::getName() {
    return name;
}

Dw::Dw(std::string line, std::string name, int value)
    : Semantic(line, Instruction::iDW), name(name), value(value) {}
std::string Dw::getName() {
    return name;
}
int Dw::getValue() {
    return value;
}

Equ::Equ(std::string line, std::string name, std::string expression)
    : Semantic(line, Instruction::iEQU), name(name), expression(expression) {}
std::string Equ::getName() {
    return name;
}
std::string Equ::getExpression() {
    return expression;
}

Assume::Assume(std::string line, std::string segmentRegister, std::string name)
    : Semantic(line, Instruction::iASSUME), segmentRegister(segmentRegister), name(name) {}
std::string Assume::getSegmentRegister() {
    return segmentRegister;
}
std::string Assume::getName() {
    return name;
}

Proc::Proc(std::string line, std::string name)
    : Semantic(line, Instruction::iPROC), name(name) {}
std::string Proc::getName() {
    return name;
}

EndP::EndP(std::string line, std::string name)
    : Semantic(line, Instruction::iENDP), name(name) {}
std::string EndP::getName() {
    return name;
}
