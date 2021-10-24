#ifndef SEMANTIC_H
#define SEMANTIC_H

#include <string>
#include <vector>

#include "Token.h"

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
        int opcode;

    protected:
        Semantic(std::string line, Instruction type);
    public:
    // ========== ONLY FOR TEST
        //Semantic(std::string line, Instruction type);
        //Semantic(std::string line, std::string name, Instruction type);
        //Semantic(std::string line, std::string name, Instruction type, std::vector<std::string> * params);
    // ==========

        std::string getLine();
        Instruction getType();
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

class MacroCall : public Semantic{
    private:
        std::string name;
        std::vector<std::vector<Token *> *> * params;
    public:
        MacroCall(std::string line, std::string name, std::vector<std::vector<Token *> *> * params);

        const std::string &getName() const;
        std::vector<std::vector<Token *> *> *getParams() const;
};

// aritmeticas

class Add : public Semantic {
    private:
        //primeiro operand : registrador AX
        std::string operand2; //registrador AX, DX ou cte
        std::vector<Token *> * expression;

    public:
        Add(std::string line, std::string operand2);
        Add(std::string line, std::vector<Token *> * expression);

        std::string getOperand2();

        std::vector<Token *> *getExpression() const;
};

class Sub : public Semantic {
    private:
        //primeiro operand : registrador AX
        std::string operand2; //AX, DX ou cte
        std::vector<Token *> * expression;
    public:
        Sub(std::string line, std::string operand2);
        Sub(std::string line, std::vector<Token *> * expression);

        std::string getOperand2();

        std::vector<Token *> *getExpression() const;
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

class Cmp : public Semantic {
    private:
        //primeiro operand : regitrador AX
        std::string operand2; //DX ou cte
        std::vector<Token *> * expression;
    public:
        Cmp(std::string line, std::string operand2);
        Cmp(std::string line, std::vector<Token *> * expression);

        std::string getOperand2();
        std::vector<Token *> *getExpression() const;
};

// logicas

class Or : public Semantic {
    private:
        //primeiro operand : registrador AX
        std::string operand2; //registrador AX, DX ou cte
        std::vector<Token *> * expression;
    public:
        Or(std::string line, std::string operand2);
        Or(std::string line, std::vector<Token *> * expression);
};

class And : public Semantic {
    private:
        //primeiro operand : registrador AX
        std::string operand2; //registrador AX, DX ou cte
        std::vector<Token *> * expression;
    public:
        And(std::string line, std::string operand2);
        And(std::string line, std::vector<Token *> * expression);
        //opcode = 23 ou 25
};

class Xor : public Semantic {
    private:
    //primeiro operand : registrador AX
    std::string operand2; //registrador AX, DX ou cte
    std::vector<Token *> * expression;
    public:
        Xor(std::string line, std::string operand2);
        Xor(std::string line, std::vector<Token *> * expression);
        //opcode = 33 ou 35
};

class Not : public Semantic {
    private:
        //int opcodeAX;         //regitrador AX
    public:
        Not(std::string line);
        //opcode = F7
};


// movimentação

class Mov : public Semantic {
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
        std::vector<Token *> * expression;         //endereço
    public:
        Jmp(std::string line, std::vector<Token *> * expression);
        std::vector<Token *> * getExpression() const;
        //opcode = EB
};

class Je : public Semantic {
    private:
        std::vector<Token *> * expression;         //endereço
    public:
        Je(std::string line, std::vector<Token *> * expression);
        std::vector<Token *> * getExpression() const;
        //opcode = 74
};

class Jnz : public Semantic {
    private:
        std::vector<Token *> * expression;         //endereço
    public:
        Jnz(std::string line, std::vector<Token *> * expression);
        std::vector<Token *> * getExpression() const;
        //opcode = 75
};

class Jz : public Semantic {
    private:
        std::vector<Token *> * expression;         //endereço
    public:
        Jz(std::string line, std::vector<Token *> * expression);
        std::vector<Token *> * getExpression() const;
        //opcode = 74
};

class Jp : public Semantic {
    private:
        std::vector<Token *> * expression;         //endereço
    public:
        Jp(std::string line, std::vector<Token *> * expression);
        std::vector<Token *> * getExpression() const;
        //opcode = 7A
};

class Call : public Semantic {
    private:
        std::vector<Token *> * expression;         //endereço
    public:
        Call(std::string line, std::vector<Token *> * expression);
        std::vector<Token *> * getExpression() const;
        //opcode = E8
};

class Int : public Semantic {
    private:
        std::vector<Token *> * expression;         //sei la oq é cte
    public:
        Int(std::string line, std::vector<Token *> * expression);
        std::vector<Token *> * getExpression() const;
        //opcode = CD
};

class Ret : public Semantic {
    private:

    public:
        Ret(std::string line);
        //opcode = C3
};

// pilha

class Pop : public Semantic {
    private:
        //o pop só recebe como operand o registrador AX
    public:
        Pop(std::string line);
};

class Push : public Semantic {
    private:
        //o push só recebe como operand o registrador AX
    public:
        Push(std::string line);
};

class Popf : public Semantic {
    private:
        //o poft não recebe nenhum operand
    public:
        Popf(std::string line);
};

class Pushf : public Semantic {
    private:
        //o pushf não recebe nenhum operand
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
class Dw : public Semantic {
    private:
        std::string name;
        std::vector<Token *> * expression; //por enquanto ta só int porque é o que tem no exemplo de entrada do trabalho
    public:
        Dw(std::string line, std::vector<Token *> * expression);
        Dw(std::string line, std::string name, std::vector<Token *> * expression);
        std::string getName();


        std::vector<Token *> *getExpression() const;
};

// declaração de constante
class Equ : public Semantic {
    private:
        std::string name;
        std::vector<Token *> * expression;     //essa expressão eu não sei como funciona, então fica assim por enquanto
    public:
        Equ(std::string line, std::string name, std::vector<Token *> *  expression);
        std::string getName();
        std::vector<Token *> * getExpression();
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



class Org : Semantic {
    private:
        std::vector<Token *> * expression;
    public:
        Org(std::string line, std::vector<Token *> *  expression);
        std::vector<Token *> * getExpression() const;
};














#endif // SEMANTIC_H
