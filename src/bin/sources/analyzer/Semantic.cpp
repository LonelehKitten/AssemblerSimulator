#include "Semantic.h"

//============================
//          SEMANTIC
//============================
Semantic::Semantic(std::string line, Instruction type)
    : line(line), type(type), opcode(nullptr) {}

std::string Semantic::getLine() {
    return line;
}

Instruction Semantic::getType() {
    return type;
}

std::vector<byte> * Semantic::getOpCode(){
    return opcode;
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
{
    opcode = new std::vector<byte>;
    opcode->push_back(0x03);
    opcode->push_back(operand2 == "dx" ? 0xC2 : 0xC0);
}

Add::Add(std::string line, std::vector<Token *> * expression) :
    Semantic(line, Instruction::iADD),
    operand2(""),
    expression(expression)
{
    opcode = new std::vector<byte>;
    opcode->push_back(0x05);    //cte
}

std::string Add::getOperand2() {
    return operand2;
}

std::vector<Token *> * Add::getExpression() const {
    return expression;
}

std::set<std::string> * Add::getSymbolSet() {

    if(expression == nullptr) return nullptr;

    std::set<std::string> * set = new std::set<std::string>;

    for(int i = 0; i < (int) expression->size(); i++){
        if(expression->at(i)->getType() == TokenTypes::tIDENTIFIER){
            set->emplace(expression->at(i)->getToken());
        }
    }

    return set;
}

//============================
//            SUB
//============================
Sub::Sub(std::string line, std::string operand2) :
    Semantic(line, Instruction::iSUB),
    operand2(operand2),
    expression(nullptr)
{
    opcode = new std::vector<byte>;
    opcode->push_back(0x2B);
    opcode->push_back(operand2 == "dx" ? 0xC2 : 0xC0);
}

Sub::Sub(std::string line, std::vector<Token *> * expression) :
    Semantic(line, Instruction::iSUB),
    operand2(""),
    expression(expression)
{
    opcode = new std::vector<byte>;
    opcode->push_back(0x2D);    //cte
}

std::string Sub::getOperand2() {
    return operand2;
}

std::vector<Token *> * Sub::getExpression() const {
    return expression;
}

std::set<std::string> * Sub::getSymbolSet() {
    
    if(expression == nullptr) return nullptr;

    std::set<std::string> * set = new std::set<std::string>;

    for(int i = 0; i < (int) expression->size(); i++){
        if(expression->at(i)->getType() == TokenTypes::tIDENTIFIER){
            set->emplace(expression->at(i)->getToken());
        }
    }

    return set;
}

//============================
//            DIV
//============================
Div::Div(std::string line, std::string operand) :
    Semantic(line, Instruction::iDIV),
    operand(operand)
{
    opcode = new std::vector<byte>;
    opcode->push_back(0xF7);
    opcode->push_back(operand == "si" ? 0xF6 : 0xF0);
}

std::string Div::getOperand() {
    return operand;
}

//============================
//            MUL
//============================
Mul::Mul(std::string line, std::string operand) :
    Semantic(line, Instruction::iMUL),
    operand(operand)
{
    opcode = new std::vector<byte>;
    opcode->push_back(0xF7);
    opcode->push_back(operand == "si" ? 0xE6 : 0xE0);
}

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
{
    opcode = new std::vector<byte>;
    opcode->push_back(0x3B);
    opcode->push_back(0xC2);
}

Cmp::Cmp(std::string line, std::vector<Token *> * expression) :
    Semantic(line, Instruction::iCMP),
    operand2(""),
    expression(expression)
{
    opcode = new std::vector<byte>;
    opcode->push_back(0x3D); // cte
}

std::string Cmp::getOperand2() {
    return operand2;
}

std::vector<Token *> * Cmp::getExpression() const
{
    return expression;
}

std::set<std::string> * Cmp::getSymbolSet() {
    
    if(expression == nullptr) return nullptr;

    std::set<std::string> * set = new std::set<std::string>;

    for(int i = 0; i < (int) expression->size(); i++){
        if(expression->at(i)->getType() == TokenTypes::tIDENTIFIER){
            set->emplace(expression->at(i)->getToken());
        }
    }

    return set;
}

//lógicas
//============================
//            OR
//============================
Or::Or(std::string line, std::string operand2) :
    Semantic(line, Instruction::iOR),
    operand2(operand2),
    expression(nullptr)
{
    opcode = new std::vector<byte>;
    opcode->push_back(0x03);
    opcode->push_back(operand2 == "dx" ? 0xC2 : 0xC0);
}

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

std::set<std::string> * Or::getSymbolSet() {
    
    if(expression == nullptr) return nullptr;

    std::set<std::string> * set = new std::set<std::string>;

    for(int i = 0; i < (int) expression->size(); i++){
        if(expression->at(i)->getType() == TokenTypes::tIDENTIFIER){
            set->emplace(expression->at(i)->getToken());
        }
    }

    return set;
}

//============================
//            AND
//============================
And::And(std::string line, std::string operand2) :
    Semantic(line, Instruction::iAND),
    operand2(operand2),
    expression(nullptr)
{
    opcode = new std::vector<byte>;
    opcode->push_back(0x23);
    opcode->push_back(operand2 == "dx" ? 0xC2 : 0xC0);
}

And::And(std::string line, std::vector<Token *> * expression) :
    Semantic(line, Instruction::iAND),
    operand2(""),
    expression(expression)
{
    opcode = new std::vector<byte>;
    opcode->push_back(0x25);    //cte
}

std::string And::getOperand2() {
    return operand2;
}

std::vector<Token *> * And::getExpression() const {
    return expression;
}

std::set<std::string> * And::getSymbolSet() {
    
    if(expression == nullptr) return nullptr;

    std::set<std::string> * set = new std::set<std::string>;

    for(int i = 0; i < (int) expression->size(); i++){
        if(expression->at(i)->getType() == TokenTypes::tIDENTIFIER){
            set->emplace(expression->at(i)->getToken());
        }
    }

    return set;
}

//============================
//            XOR
//============================
Xor::Xor(std::string line, std::string operand2) :
    Semantic(line, Instruction::iXOR),
    operand2(operand2),
    expression(nullptr)
{
    opcode = new std::vector<byte>;
    opcode->push_back(0x33);
    opcode->push_back(operand2 == "dx" ? 0xC2 : 0xC0);
}

Xor::Xor(std::string line, std::vector<Token *> * expression) :
    Semantic(line, Instruction::iXOR),
    operand2(""),
    expression(expression)
{
    opcode = new std::vector<byte>;
    opcode->push_back(0x35); //cte
}

std::string Xor::getOperand2() {
    return operand2;
}

std::vector<Token *> * Xor::getExpression() const {
    return expression;
}

std::set<std::string> * Xor::getSymbolSet() {
    
    if(expression == nullptr) return nullptr;

    std::set<std::string> * set = new std::set<std::string>;

    for(int i = 0; i < (int) expression->size(); i++){
        if(expression->at(i)->getType() == TokenTypes::tIDENTIFIER){
            set->emplace(expression->at(i)->getToken());
        }
    }

    return set;
}

//============================
//            NOT
//============================
Not::Not(std::string line) : Semantic(line, Instruction::iNOT) {
    opcode = new std::vector<byte>;
    opcode->push_back(0xF7);
    opcode->push_back(0xD0);
};

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
{
    opcode = new std::vector<byte>;

    if(operand1 == "sp" && operand2 == "ax") {
        opcode->push_back(0x8B);
        opcode->push_back(0xE0);
    }
    else if(operand1 == "ss" && operand2 == "ax") {
        opcode->push_back(0x8E);
        opcode->push_back(0xD0);
    }
    else if(operand1 == "ds" && operand2 == "ax") {
        opcode->push_back(0x8E);
        opcode->push_back(0xD8);
    }
    else if(operand1 == "ax" && operand2 == "sp") {
        opcode->push_back(0x8B);
        opcode->push_back(0xC4);
    }
    else if(operand1 == "ax" && operand2 == "ss") {
        opcode->push_back(0x8C);
        opcode->push_back(0xD0);
    }
    else if(operand1 == "ax" && operand2 == "ds") {
        opcode->push_back(0x8C);
        opcode->push_back(0xD8);
    }
    else if(operand1 == "ax" && operand2 == "cs") {
        opcode->push_back(0x8C);
        opcode->push_back(0xC8);
    }
    else if(operand1 == "ax" && operand2 == "dx") {
        opcode->push_back(0x8B);
        opcode->push_back(0xC2);
    }
    else if(operand1 == "ax" && operand2 == "si") {
        opcode->push_back(0x8B);
        opcode->push_back(0xC6);
    }
    else if(operand1 == "dx" && operand2 == "ax") {
        opcode->push_back(0x8B);
        opcode->push_back(0xD0);
    }
    else if(operand1 == "si" && operand2 == "ax") {
        opcode->push_back(0x8B);
        opcode->push_back(0xF0);
    }
    
}

Mov::Mov(std::string line, std::vector<Token *> * expression1, std::string operand2, bool indexed) :
    Semantic(line, Instruction::iMOV),
    operand1(""),
    operand2(operand2),
    expression1(expression1),
    expression2(nullptr),
    indexed(indexed)
{
    opcode = new std::vector<byte>;

    if(expression1->empty()) {
        opcode->push_back(0x89);
        opcode->push_back(0x04);
        return;
    }

    if(indexed) {
        opcode->push_back(0x89);
        opcode->push_back(0x84);
        return;
    }

    opcode->push_back(0xA3);

}

Mov::Mov(std::string line, std::string operand1, std::vector<Token *> * expression2, bool indexed) :
    Semantic(line, Instruction::iMOV),
    operand1(operand1),
    operand2(""),
    expression1(nullptr),
    expression2(expression2),
    indexed(indexed)
{
    if(indexed) {
        opcode = new std::vector<byte>;
        opcode->push_back(0x8B);
        if(expression2->empty()) {
            opcode->push_back(0x04);
            return;
        }
        opcode->push_back(0x84);
    }
}


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

std::set<std::string> * Mov::getSymbolSet() {
    
    if(expression1 == nullptr && expression2 == nullptr) return nullptr;

    std::set<std::string> * set = new std::set<std::string>;

    for(int i = 0; expression1 != nullptr && i < (int) expression1->size(); i++){
        if(expression1->at(i)->getType() == TokenTypes::tIDENTIFIER){
            set->emplace(expression1->at(i)->getToken());
        }
    }

    for(int i = 0; expression2 != nullptr && i < (int) expression2->size(); i++){
        if(expression2->at(i)->getType() == TokenTypes::tIDENTIFIER){
            set->emplace(expression2->at(i)->getToken());
        }
    }

    return set;
}

//pilha

//============================
//            POP
//============================
Pop::Pop(std::string line)
    : Semantic(line, Instruction::iPOP) 
{
    opcode = new std::vector<byte>;
    opcode->push_back(0x58);
}

//============================
//            PUSH
//============================
Push::Push(std::string line)
    : Semantic(line, Instruction::iPUSH) 
{
    opcode = new std::vector<byte>;
    opcode->push_back(0x50);
}

//============================
//            POPF
//============================
Popf::Popf(std::string line)
    : Semantic(line, Instruction::iPOPF) 
{
    opcode = new std::vector<byte>;
    opcode->push_back(0x9D);
}

//============================
//           PUSHF
//============================
Pushf::Pushf(std::string line)
    : Semantic(line, Instruction::iPUSHF) 
{
    opcode = new std::vector<byte>;
    opcode->push_back(0x9C);
}

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

std::set<std::string> * Dw::getSymbolSet() {
    
    if(defaultValue == nullptr || length == nullptr) return nullptr;

    std::set<std::string> * set = new std::set<std::string>;

    for(int i = 0; i < (int) defaultValue->size(); i++){
        if(defaultValue->at(i)->getType() == TokenTypes::tIDENTIFIER){
            set->emplace(defaultValue->at(i)->getToken());
        }
    }

    for(int i = 0; i < (int) length->size(); i++){
        if(length->at(i)->getType() == TokenTypes::tIDENTIFIER){
            set->emplace(length->at(i)->getToken());
        }
    }

    return set;
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

std::set<std::string> * Equ::getSymbolSet() {
    
    if(expression == nullptr) return nullptr;

    std::set<std::string> * set = new std::set<std::string>;

    for(int i = 0; i < (int) expression->size(); i++){
        if(expression->at(i)->getType() == TokenTypes::tIDENTIFIER){
            set->emplace(expression->at(i)->getToken());
        }
    }

    return set;
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
{
    opcode = new std::vector<byte>;
    opcode->push_back(0xEB);
}

std::vector<Token *> * Jmp::getExpression() const {
    return expression;
}

std::set<std::string> * Jmp::getSymbolSet() {
    
    if(expression == nullptr) return nullptr;

    std::set<std::string> * set = new std::set<std::string>;

    for(int i = 0; i < (int) expression->size(); i++){
        if(expression->at(i)->getType() == TokenTypes::tIDENTIFIER){
            set->emplace(expression->at(i)->getToken());
        }
    }

    return set;
}

//============================
//            JE
//============================
Je::Je(std::string line, std::vector<Token *> * expression) :
    Semantic(line, Instruction::iJE),
    expression(expression)
{
    opcode = new std::vector<byte>;
    opcode->push_back(0x74);
}

std::vector<Token *> * Je::getExpression() const {
    return expression;
}

std::set<std::string> * Je::getSymbolSet() {
    
    if(expression == nullptr) return nullptr;

    std::set<std::string> * set = new std::set<std::string>;

    for(int i = 0; i < (int) expression->size(); i++){
        if(expression->at(i)->getType() == TokenTypes::tIDENTIFIER){
            set->emplace(expression->at(i)->getToken());
        }
    }

    return set;
}

//============================
//            JNZ
//============================
Jnz::Jnz(std::string line, std::vector<Token *> * expression) :
    Semantic(line, Instruction::iJNZ), expression(expression)
{
    opcode = new std::vector<byte>;
    opcode->push_back(0x75);
}

std::vector<Token *> * Jnz::getExpression() const {
    return expression;
}

std::set<std::string> * Jnz::getSymbolSet() {
    
    if(expression == nullptr) return nullptr;

    std::set<std::string> * set = new std::set<std::string>;

    for(int i = 0; i < (int) expression->size(); i++){
        if(expression->at(i)->getType() == TokenTypes::tIDENTIFIER){
            set->emplace(expression->at(i)->getToken());
        }
    }

    return set;
}

//============================
//            JZ
//============================
Jz::Jz(std::string line, std::vector<Token *> * expression) :
    Semantic(line, Instruction::iJZ), expression(expression)
{
    opcode = new std::vector<byte>;
    opcode->push_back(0x74);
}

std::vector<Token *> * Jz::getExpression() const {
    return expression;
}

std::set<std::string> * Jz::getSymbolSet() {
    
    if(expression == nullptr) return nullptr;

    std::set<std::string> * set = new std::set<std::string>;

    for(int i = 0; i < (int) expression->size(); i++){
        if(expression->at(i)->getType() == TokenTypes::tIDENTIFIER){
            set->emplace(expression->at(i)->getToken());
        }
    }

    return set;
}

//============================
//            JP
//============================
Jp::Jp(std::string line, std::vector<Token *> * expression) :
    Semantic(line, Instruction::iJP), expression(expression)
{
    opcode = new std::vector<byte>;
    opcode->push_back(0x7A);
}

std::vector<Token *> * Jp::getExpression() const {
    return expression;
}

std::set<std::string> * Jp::getSymbolSet() {
    
    if(expression == nullptr) return nullptr;

    std::set<std::string> * set = new std::set<std::string>;

    for(int i = 0; i < (int) expression->size(); i++){
        if(expression->at(i)->getType() == TokenTypes::tIDENTIFIER){
            set->emplace(expression->at(i)->getToken());
        }
    }

    return set;
}

//============================
//            CALL
//============================
Call::Call(std::string line, std::vector<Token *> * expression) :
    Semantic(line, Instruction::iCALL), expression(expression)
{
    opcode = new std::vector<byte>;
    opcode->push_back(0xE8);
}

std::vector<Token *> * Call::getExpression() const {
    return expression;
}

std::set<std::string> * Call::getSymbolSet() {
    
    if(expression == nullptr) return nullptr;

    std::set<std::string> * set = new std::set<std::string>;

    for(int i = 0; i < (int) expression->size(); i++){
        if(expression->at(i)->getType() == TokenTypes::tIDENTIFIER){
            set->emplace(expression->at(i)->getToken());
        }
    }

    return set;
}

//============================
//            INT
//============================
Int::Int(std::string line, std::vector<Token *> * expression) :
    Semantic(line, Instruction::iINT), expression(expression)
{
    opcode = new std::vector<byte>;
    opcode->push_back(0xCD);    //cte
}

std::vector<Token *> * Int::getExpression() const {
    return expression;
}

std::set<std::string> * Int::getSymbolSet() {
    
    if(expression == nullptr) return nullptr;

    std::set<std::string> * set = new std::set<std::string>;

    for(int i = 0; i < (int) expression->size(); i++){
        if(expression->at(i)->getType() == TokenTypes::tIDENTIFIER){
            set->emplace(expression->at(i)->getToken());
        }
    }

    return set;
}

//============================
//            RET
//============================
Ret::Ret(std::string line) : Semantic(line, Instruction::iRET)
{
    opcode = new std::vector<byte>;
    opcode->push_back(0xC3);
}


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

std::set<std::string> * Org::getSymbolSet() {
    
    if(expression == nullptr) return nullptr;

    std::set<std::string> * set = new std::set<std::string>;

    for(int i = 0; i < (int) expression->size(); i++){
        if(expression->at(i)->getType() == TokenTypes::tIDENTIFIER){
            set->emplace(expression->at(i)->getToken());
        }
    }

    return set;
}
