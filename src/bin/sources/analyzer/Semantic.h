#ifndef SEMANTIC_H
#define SEMANTIC_H

#include <string>
#include <vector>
#include <set>

#include "Token.h"
#include "../Enums.h"

/*
 * tipo e linha
 * operador / parametros / operand
 */

// intruções:
enum Instruction{
    //aritmeticas
    iADD,
    iSUB,
    iDIV,
    iMUL,
    iCMP,
    //logicas
    iOR,
    iAND,
    iXOR,
    iNOT,
    //desvio
    iJMP,
    iJE,
    iJNZ,
    iJZ,
    iJP,
    iCALL,
    iINT,
    iRET,
    //movimentação
    iMOV,
    //pilha
    iPOP,
    iPUSH,
    iPOPF,
    iPUSHF,

    //instruções ao processo de montagem
    iEND,
    iSEGMENT,
    iENDS,
    iDW,
    //iDUP, n precisa
    iEQU,
    iORG,
    //iOFFSET, n precisa
    iASSUME,
    iPROC,
    iENDP,
    iMACRO,
    iENDM,
    iMACROCALL,
    iMACROCONTENT,
    iLABEL,
    iINVALID,
    iHALT
};

class Semantic {
    private:
        std::string line;      //linha do arquivo
        Instruction type;

    protected:
        std::vector<byte> * opcode;
        Semantic(std::string line, Instruction type);
    public:

        std::string getLine();
        Instruction getType();
        std::vector<byte> * getOpCode();

};

struct ISemantic {
    virtual std::set<std::string> * getSymbolSet() = 0;
};

class Invalid : public Semantic {
    public:
        Invalid(std::string line);
};

// MACRO

class Macro : public Semantic {
    private:
        std::string name;
        std::vector<std::string> * params;
    public:
        Macro(std::string line, std::string name);
        Macro(std::string line, std::string name, std::vector<std::string> * params);

        std::string getName();
        std::vector<std::string> * getParams();
};

class EndM : public Semantic {
    private:

    public:
        EndM(std::string line);
};

class MacroContent : public Semantic {
    public:
        MacroContent(const std::string line);
};

class MacroCall : public Semantic {
    private:
        std::string name;
        std::vector<std::vector<Token *> *> * params;
    public:
        MacroCall(std::string line, std::string name, std::vector<std::vector<Token *> *> * params);

        const std::string &getName() const;
        std::vector<std::vector<Token *> *> *getParams() const;
};

// aritmeticas

class Add : public Semantic, ISemantic {
    private:
        //primeiro operand : registrador AX
        std::string operand2; //registrador AX, DX ou cte
        std::vector<Token *> * expression;

    public:
        Add(std::string line, std::string operand2);
        Add(std::string line, std::vector<Token *> * expression);

        std::string getOperand2();
        std::vector<Token *> *getExpression() const;

        std::set<std::string> * getSymbolSet();

};

class Sub : public Semantic, ISemantic {
    private:
        //primeiro operand : registrador AX
        std::string operand2; //AX, DX ou cte

        std::vector<Token *> * expression;
    public:
        Sub(std::string line, std::string operand2);
        Sub(std::string line, std::vector<Token *> * expression);

        std::string getOperand2();
        std::vector<Token *> *getExpression() const;

        std::set<std::string> * getSymbolSet();

};

class Div : public Semantic {
    private:
        std::string operand; //AX ou SI
    public:
        Div(std::string line, std::string operand);
        std::string getOperand();
};

class Mul : public Semantic {
    private:
        std::string operand; //AX ou SI
    public:
        Mul(std::string line, std::string operand);
        std::string getOperand();
};

class Cmp : public Semantic, ISemantic {
    private:
        //primeiro operand : regitrador AX
        std::string operand2; //DX ou cte
        std::vector<Token *> * expression;
    public:
        Cmp(std::string line, std::string operand2);
        Cmp(std::string line, std::vector<Token *> * expression);

        std::string getOperand2();
        std::vector<Token *> *getExpression() const;

        std::set<std::string> * getSymbolSet();
};

// logicas

class Or : public Semantic, ISemantic {
    private:
        //primeiro operand : registrador AX
        std::string operand2; //registrador AX, DX ou cte
        std::vector<Token *> * expression;
    public:
        Or(std::string line, std::string operand2);
        Or(std::string line, std::vector<Token *> * expression);

        std::string getOperand2();
        std::vector<Token *> *getExpression() const;

        std::set<std::string> * getSymbolSet();
};

class And : public Semantic, ISemantic {
    private:
        //primeiro operand : registrador AX
        std::string operand2; //registrador AX, DX ou cte
        std::vector<Token *> * expression;
    public:
        And(std::string line, std::string operand2);
        And(std::string line, std::vector<Token *> * expression);
        //opcode = 23 ou 25

        std::string getOperand2();
        std::vector<Token *> *getExpression() const;

        std::set<std::string> * getSymbolSet();
};

class Xor : public Semantic, ISemantic {
    private:
    //primeiro operand : registrador AX
    std::string operand2; //registrador AX, DX ou cte
    std::vector<Token *> * expression;
    public:
        Xor(std::string line, std::string operand2);
        Xor(std::string line, std::vector<Token *> * expression);
        //opcode = 33 ou 35

        std::string getOperand2();
        std::vector<Token *> *getExpression() const;

        std::set<std::string> * getSymbolSet();
};

class Not : public Semantic{
    private:
        //int opcodeAX;         //regitrador AX
    public:
        Not(std::string line);
        //opcode = F7
};


// movimentação

class Mov : public Semantic, ISemantic {
    private:
        std::string operand1, operand2;
        std::vector<Token *> * expression1, * expression2;
        bool indexed;

    public:
        Mov(std::string line, std::string operand1, std::string operand2);
        Mov(std::string line, std::vector<Token *> * expression1, std::string operand2, bool indexed);
        Mov(std::string line, std::string operand1, std::vector<Token *> * expression1, bool indexed);

        std::string getOperand1() const;
        std::string getOperand2() const;

        std::vector<Token *> * getExpression1() const;
        std::vector<Token *> * getExpression2() const;

        bool isIndexed() const;

        std::set<std::string> * getSymbolSet();
};

// desvio
/*
Jmp(std::string line);
Je(std::string line);
Jnz(std::string line);
Jz(std::string line);
Jp(std::string line);
Int(std::string line);
Call(std::string line);
Ret(std::string line);

*/
class Jmp : public Semantic {
    private:
        std::string label;         //endereço
    public:
        Jmp(std::string line, std::string label);
        std::string getLabel() const;
        
};

class Je : public Semantic {
    private:
        std::string label;         //endereço
    public:
        Je(std::string line, std::string label);
        std::string getLabel() const;
        
};

class Jnz : public Semantic {
    private:
        std::string label;         //endereço
    public:
        Jnz(std::string line, std::string label);
        std::string getLabel() const;

};

class Jz : public Semantic {
    private:
        std::string label;         //endereço
    public:
        Jz(std::string line, std::string label);
        std::string getLabel() const;
        
};

class Jp : public Semantic {
    private:
        std::string label;         //endereço
    public:
        Jp(std::string line, std::string label);
        std::string getLabel() const;
};

class Call : public Semantic {
    private:
        std::string label;         //endereço
    public:
        Call(std::string line, std::string label);
        std::string getLabel() const;
};

class Int : public Semantic, ISemantic {
    private:
        std::vector<Token *> * expression; 
    public:
        Int(std::string line, std::vector<Token *> * expression);
        std::vector<Token *> * getExpression() const;
        
        std::set<std::string> * getSymbolSet();
};

class Ret : public Semantic{
    public:
        Ret(std::string line);
};

// pilha

class Pop : public Semantic {
    public:
        Pop(std::string line);
};

class Push : public Semantic {
    public:
        Push(std::string line);
};

class Popf : public Semantic {
    public:
        Popf(std::string line);
};

class Pushf : public Semantic {
    public:
        Pushf(std::string line);
};

// montagem

class End : public Semantic {
    private:
        std::string name;   //nome do ponto de entrada
    public:
        End(std::string line, std::string name);
        std::string getName();
};

class Segment : public Semantic {
    private:
        std::string name;   //nome do segmento
    public:
        Segment(std::string line, std::string name);
        std::string getName();
};

class EndS : public Semantic {
    private:
        std::string name;   //nome do segmento
    public:
        EndS(std::string line, std::string name);
        std::string getName();
};

// declaração de variavel do tamanho de uma word, pode ser inteiro, uma constante, end de expressao, ...
class Dw : public Semantic, ISemantic {
    private:
        std::string name;
        std::vector<Token *> * defaultValue, * length; //por enquanto ta só int porque é o que tem no exemplo de entrada do trabalho
    public:
        Dw(std::string line, std::vector<Token *> * defaultValue);
        Dw(std::string line, std::vector<Token *> * defaultValue, std::vector<Token *> * length);
        Dw(std::string line, std::string name, std::vector<Token *> * defaultValue);
        Dw(std::string line, std::string name, std::vector<Token *> * defaultValue, std::vector<Token *> * length);

        std::string getName();
        std::vector<Token *> *getDefaultValue() const;
        std::vector<Token *> *getLength() const;

        bool isArray() const;

        std::set<std::string> * getSymbolSet();
};

// declaração de constante
class Equ : public Semantic, ISemantic {
    private:
        std::string name;
        std::vector<Token *> * expression;     //essa expressão eu não sei como funciona, então fica assim por enquanto
    public:
        Equ(std::string line, std::string name, std::vector<Token *> *  expression);
        std::string getName();
        std::vector<Token *> * getExpression();

        std::set<std::string> * getSymbolSet();
};

class Assume : public Semantic {
    private:
        std::string segmentRegister;        //registrador de segmento
        std::string name;                   //nome de segmento
    public:
        Assume(std::string line, std::string segmentRegister, std::string name);
        std::string getSegmentRegister();
        std::string getName();
};

class Proc : public Semantic {
    private:
        std::string name;
    public:
        Proc(std::string line, std::string name);
        std::string getName();
};

class EndP : public Semantic {
    private:
        std::string name;
    public:
        EndP(std::string line, std::string name);
        std::string getName();
};

class Label : public Semantic {
    private:
        std::string name;
        Semantic * semantic;
    public:
        Label(std::string line, std::string name, Semantic * semantic);
        std::string getName();
        Semantic * getSemantic();
};


class Halt : public Semantic {
    public:
        Halt(std::string line);
};


class Org : public Semantic, ISemantic {
    private:
        std::vector<Token *> * expression;
    public:
        Org(std::string line, std::vector<Token *> *  expression);
        std::vector<Token *> * getExpression() const;

        std::set<std::string> * getSymbolSet();
};

#endif // SEMANTIC_H
