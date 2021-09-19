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

Invalid::Invalid(std::string line)
    : Semantic(line, Instruction::iINVALID) {}

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

EndM::EndM(std::string line) : Semantic(line, Instruction::iENDM) {}

//instructions


Add::Add(std::string line, std::string segundoOperando) :
    Semantic(line, Instruction::iADD),
    segundoOperando(segundoOperando),
    expression(nullptr)
{}

Add::Add(std::string line, std::vector<Token *> * expression) :
    Semantic(line, Instruction::iADD),
    segundoOperando(""),
    expression(expression)
{}

std::string Add::getSegundoOperando() {
    return segundoOperando;
}

std::vector<Token *> * Add::getExpression() const
{
    return expression;
}

Sub::Sub(std::string line, std::string segundoOperando) :
    Semantic(line, Instruction::iSUB),
    segundoOperando(segundoOperando),
    expression(nullptr)
{}

Sub::Sub(std::string line, std::vector<Token *> * expression) :
    Semantic(line, Instruction::iSUB),
    segundoOperando(""),
    expression(expression)
{}

std::string Sub::getSegundoOperando() {
    return segundoOperando;
}

std::vector<Token *> * Sub::getExpression() const
{
    return expression;
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

Mov::Mov(std::string line, std::string operand1, std::string operand2) :
    Semantic(line, Instruction::iMOV),
    operand1(operand1),
    operand2(operand2),
    expression1(nullptr),
    expression2(nullptr),
    indexed(false)
{}
Mov::Mov(std::string line, std::vector<Token *> * expression1, std::string operand2, bool indexed) :
    Semantic(line, Instruction::iMOV),
    operand1(""),
    operand2(operand2),
    expression1(expression1),
    expression2(nullptr),
    indexed(indexed)
{}


Mov::Mov(std::string line, std::string operand1, std::vector<Token *> * expression2, bool indexed) :
    Semantic(line, Instruction::iMOV),
    operand1(operand1),
    operand2(""),
    expression1(nullptr),
    expression2(expression2),
    indexed(indexed)
{}


std::string Mov::getOperand1() const
{
    return operand1;
}

std::string Mov::getOperand2() const
{
    return operand2;
}

std::vector<Token *> * Mov::getExpression1() const
{
    return expression1;
}

std::vector<Token *> * Mov::getExpression2() const
{
    return expression2;
}

bool Mov::isIndexed() const
{
    return indexed;
}

//pilha

Pop::Pop(std::string line)
    : Semantic(line, Instruction::iPOP) {}

Push::Push(std::string line)
    : Semantic(line, Instruction::iPUSH) {}

Popf::Popf(std::string line)
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


Dw::Dw(std::string line, std::vector<Token *> * expression) :
    Semantic(line, Instruction::iDW),
    name(""),
    expression(expression)
{}

Dw::Dw(std::string line, std::string name, std::vector<Token *> * expression) :
    Semantic(line, Instruction::iDW),
    name(name),
    expression(expression)
{}

std::string Dw::getName() {
    return name;
}

std::vector<Token *> *Dw::getExpression() const
{
    return expression;
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


Label::Label(std::string line, std::string name)
    : Semantic(line, Instruction::iLABEL), name(name) {}
std::string Label::getName() {
    return name;
}


MacroCall::MacroCall(std::string line, std::vector<std::vector<Token *> *> * params) :
    Semantic(line, Instruction::iMACROCALL), params(params) {};

const std::string &MacroCall::getName() const
{
    return name;
}

std::vector<std::vector<Token *> *> * MacroCall::getParams() const
{
    return params;
}

MacroContent::MacroContent(const std::string line) : Semantic(line, Instruction::iMACROCONTENT)
{}

