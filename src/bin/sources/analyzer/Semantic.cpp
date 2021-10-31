#include "Semantic.h"

//============================
//          SEMANTIC
//============================
Semantic::Semantic(std::string line, Instruction type)
    : line(line), type(type) {}

std::string Semantic::getLine() {
    return line;
}

Instruction Semantic::getType() {
    return type;
}

Invalid::Invalid(std::string line)
    : Semantic(line, Instruction::iINVALID) {}

//MACRO
//============================
//           MACRO
//============================
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

//============================
//            ENDM
//============================
EndM::EndM(std::string line) : Semantic(line, Instruction::iENDM) {}

//instructions

//============================
//            ADD
//============================
Add::Add(std::string line, std::string operand2) :
    Semantic(line, Instruction::iADD),
    operand2(operand2),
    expression(nullptr)
{}

Add::Add(std::string line, std::vector<Token *> * expression) :
    Semantic(line, Instruction::iADD),
    operand2(""),
    expression(expression)
{}

std::string Add::getOperand2() {
    return operand2;
}

std::vector<Token *> * Add::getExpression() const {
    return expression;
}

//============================
//            SUB
//============================
Sub::Sub(std::string line, std::string operand2) :
    Semantic(line, Instruction::iSUB),
    operand2(operand2),
    expression(nullptr)
{}

Sub::Sub(std::string line, std::vector<Token *> * expression) :
    Semantic(line, Instruction::iSUB),
    operand2(""),
    expression(expression)
{}

std::string Sub::getOperand2() {
    return operand2;
}

std::vector<Token *> * Sub::getExpression() const {
    return expression;
}

//============================
//            DIV
//============================
Div::Div(std::string line, std::string operand) :
    Semantic(line, Instruction::iDIV),
    operand(operand)
{}

std::string Div::getOperand() {
    return operand;
}

//============================
//            MUL
//============================
Mul::Mul(std::string line, std::string operand) :
    Semantic(line, Instruction::iMUL),
    operand(operand)
{}

std::string Mul::getOperand() {
    return operand;
}

//============================
//            CMP
//============================
Cmp::Cmp(std::string line, std::string operand2) :
    Semantic(line, Instruction::iCMP),
    operand2(operand2),
    expression(nullptr)
{}

Cmp::Cmp(std::string line, std::vector<Token *> * expression) :
    Semantic(line, Instruction::iCMP),
    operand2(""),
    expression(expression)
{}

std::string Cmp::getOperand2() {
    return operand2;
}

std::vector<Token *> * Cmp::getExpression() const
{
    return expression;
}

//lógicas
//============================
//            OR
//============================
Or::Or(std::string line, std::string operand2) :
    Semantic(line, Instruction::iOR),
    operand2(operand2),
    expression(nullptr)
{}

Or::Or(std::string line, std::vector<Token *> * expression) :
    Semantic(line, Instruction::iOR),
    operand2(""),
    expression(expression)
{}

std::string Or::getOperand2() {
    return operand2;
}

std::vector<Token *> * Or::getExpression() const {
    return expression;
}

//============================
//            AND
//============================
And::And(std::string line, std::string operand2) :
    Semantic(line, Instruction::iAND),
    operand2(operand2),
    expression(nullptr)
{}

And::And(std::string line, std::vector<Token *> * expression) :
    Semantic(line, Instruction::iAND),
    operand2(""),
    expression(expression)
{}

std::string And::getOperand2() {
    return operand2;
}

std::vector<Token *> * And::getExpression() const {
    return expression;
}

//============================
//            XOR
//============================
Xor::Xor(std::string line, std::string operand2) :
    Semantic(line, Instruction::iXOR),
    operand2(operand2),
    expression(nullptr)
{}

Xor::Xor(std::string line, std::vector<Token *> * expression) :
    Semantic(line, Instruction::iXOR),
    operand2(""),
    expression(expression)
{}

std::string Xor::getOperand2() {
    return operand2;
}

std::vector<Token *> * Xor::getExpression() const {
    return expression;
}

//============================
//            NOT
//============================
Not::Not(std::string line) : Semantic(line, Instruction::iNOT) {};

// movimentação
//============================
//            MOV
//============================
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


std::string Mov::getOperand1() const {
    return operand1;
}

std::string Mov::getOperand2() const {
    return operand2;
}

std::vector<Token *> * Mov::getExpression1() const {
    return expression1;
}

std::vector<Token *> * Mov::getExpression2() const {
    return expression2;
}

bool Mov::isIndexed() const {
    return indexed;
}

//pilha

//============================
//            POP
//============================
Pop::Pop(std::string line)
    : Semantic(line, Instruction::iPOP) {}

//============================
//            PUSH
//============================
Push::Push(std::string line)
    : Semantic(line, Instruction::iPUSH) {}

//============================
//            POPF
//============================
Popf::Popf(std::string line)
    : Semantic(line, Instruction::iPOPF) {}

//============================
//           PUSHF
//============================
Pushf::Pushf(std::string line)
    : Semantic(line, Instruction::iPUSHF) {}

//montagem

//============================
//            END
//============================
End::End(std::string line, std::string name) :
    Semantic(line, Instruction::iEND),
    name(name)
{}

std::string End::getName() {
    return name;
}


//============================
//          SEGMENT
//============================
Segment::Segment(std::string line, std::string name) :
    Semantic(line, Instruction::iSEGMENT),
    name(name)
{}

std::string Segment::getName() {
    return name;
}


//============================
//            ENDS
//============================
EndS::EndS(std::string line, std::string name) :
    Semantic(line, Instruction::iENDS),
    name(name)
{}

std::string EndS::getName() {
    return name;
}


//============================
//            DW
//============================
Dw::Dw(std::string line, std::vector<Token *> * defaultValue) :
    Semantic(line, Instruction::iDW),
    name(""),
    defaultValue(defaultValue),
    length(nullptr)
{}

Dw::Dw(std::string line, std::vector<Token *> * defaultValue, std::vector<Token *> * length) :
    Semantic(line, Instruction::iDW),
    name(""),
    defaultValue(defaultValue),
    length(length)
{}

Dw::Dw(std::string line, std::string name, std::vector<Token *> * defaultValue) :
    Semantic(line, Instruction::iDW),
    name(name),
    defaultValue(defaultValue),
    length(nullptr)
{}

Dw::Dw(std::string line, std::string name, std::vector<Token *> * defaultValue, std::vector<Token *> * length) :
    Semantic(line, Instruction::iDW),
    name(name),
    defaultValue(defaultValue),
    length(length)
{}

std::string Dw::getName() {
    return name;
}

std::vector<Token *> * Dw::getDefaultValue() const {
    return defaultValue;
}

std::vector<Token *> * Dw::getLength() const {
    return length;
}

bool Dw::isArray() const {
    return length != nullptr;
}


//============================
//            EQU
//============================
Equ::Equ(std::string line, std::string name, std::vector<Token *> * expression) :
    Semantic(line, Instruction::iEQU),
    name(name),
    expression(expression)
{}

std::string Equ::getName() {
    return name;
}

std::vector<Token *> * Equ::getExpression() {
    return expression;
}


//============================
//            ASSUME
//============================
Assume::Assume(std::string line, std::string segmentRegister, std::string name) :
    Semantic(line, Instruction::iASSUME),
    segmentRegister(segmentRegister),
    name(name)
{}

std::string Assume::getSegmentRegister() {
    return segmentRegister;
}

std::string Assume::getName() {
    return name;
}


//============================
//            PROC
//============================
Proc::Proc(std::string line, std::string name) :
    Semantic(line, Instruction::iPROC),
    name(name)
{}

std::string Proc::getName() {
    return name;
}


//============================
//            ENDP
//============================
EndP::EndP(std::string line, std::string name) :
    Semantic(line, Instruction::iENDP),
    name(name)
{}

std::string EndP::getName() {
    return name;
}

//============================
//            LABEL
//============================
Label::Label(std::string line, std::string name, Semantic * semantic) :
    Semantic(line, Instruction::iLABEL),
    name(name),
    semantic(semantic)
{}

std::string Label::getName() {
    return name;
}

Semantic * Label::getSemantic() {
    return semantic;
}


//============================
//         MACROCALL
//============================
MacroCall::MacroCall(std::string line, std::string name, std::vector<std::vector<Token *> *> * params) :
    Semantic(line, Instruction::iMACROCALL),
    name(name),
    params(params)
{};

const std::string &MacroCall::getName() const {
    return name;
}

std::vector<std::vector<Token *> *> * MacroCall::getParams() const {
    return params;
}

//============================
//        MACROCONTENT
//============================
MacroContent::MacroContent(const std::string line) : Semantic(line, Instruction::iMACROCONTENT)
{}

// desvio

//============================
//            JMP
//============================
Jmp::Jmp(std::string line, std::vector<Token *> * expression) :
    Semantic(line, Instruction::iJMP),
    expression(expression)
{}

std::vector<Token *> * Jmp::getExpression() const {
    return expression;
}

//============================
//            JE
//============================
Je::Je(std::string line, std::vector<Token *> * expression) :
    Semantic(line, Instruction::iJE),
    expression(expression)
{}

std::vector<Token *> * Je::getExpression() const {
    return expression;
}

//============================
//            JNZ
//============================
Jnz::Jnz(std::string line, std::vector<Token *> * expression) :
    Semantic(line, Instruction::iJNZ), expression(expression)
{}

std::vector<Token *> * Jnz::getExpression() const {
    return expression;
}

//============================
//            JZ
//============================
Jz::Jz(std::string line, std::vector<Token *> * expression) :
    Semantic(line, Instruction::iJZ), expression(expression)
{}

std::vector<Token *> * Jz::getExpression() const {
    return expression;
}

//============================
//            JP
//============================
Jp::Jp(std::string line, std::vector<Token *> * expression) :
    Semantic(line, Instruction::iJP), expression(expression)
{}

std::vector<Token *> * Jp::getExpression() const {
    return expression;
}

//============================
//            CALL
//============================
Call::Call(std::string line, std::vector<Token *> * expression) :
    Semantic(line, Instruction::iCALL), expression(expression)
{}

std::vector<Token *> * Call::getExpression() const {
    return expression;
}

//============================
//            INT
//============================
Int::Int(std::string line, std::vector<Token *> * expression) :
    Semantic(line, Instruction::iINT), expression(expression)
{}

std::vector<Token *> * Int::getExpression() const {
    return expression;
}

//============================
//            RET
//============================
Ret::Ret(std::string line) : Semantic(line, Instruction::iRET) {}


//============================
//            HALT
//============================
Halt::Halt(std::string line) : Semantic(line, Instruction::iHALT) {}


//============================
//            ORG
//============================
Org::Org(std::string line, std::vector<Token *> * expression) :
    Semantic(line, Instruction::iINT),
    expression(expression)
{}

std::vector<Token *> * Org::getExpression() const {
    return expression;
}
