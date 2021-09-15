#ifndef SEMANTIC_H
#define SEMANTIC_H

#include <string>
#include <vector>
/*
 * tipo e linha
 * operador / parametros / operando
 */

//intruções:
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
    iENDM
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

//MACRO

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

//aritmeticas

class Add : public Semantic {
    private:
        //primeiro operando : registrador AX
        std::string segundoOperando; //registrador AX, DX ou cte
    public:
        Add(std::string line, std::string segundoOperando);
        std::string getSegundoOperando();
};

class Sub : public Semantic {
    private:
        //primeiro operando : registrador AX
        std::string segundoOperando; //AX, DX ou cte
    public:
        Sub(std::string line, std::string segundoOperando);
        std::string getSegundoOperando();
};

class Div : public Semantic {
    private:
        std::string operando; //AX ou SI
    public:
        Div(std::string line, std::string operando);
        std::string getOperando();
};

class Mul : public Semantic {
    private:
        std::string operando; //AX ou SI
    public:
        Mul(std::string line, std::string operando);
        std::string getOperando();
};

class Cmp : public Semantic {
    private:
        //primeiro operando : regitrador AX
        std::string segundoOperando; //DX ou cte
    public:
        Cmp(std::string line, std::string segundoOperando);
        std::string getSegundoOperando();
};


//movimentação

class Mov : public Semantic {
    private:
        std::vector<std::string> * operandos;   //AX, DX ou cte
    public:
        Mov(std::string line, std::vector<std::string> * operandos);
        std::vector<std::string> * getOperandos();
};

//pilha

class Pop : public Semantic {
    private:
        //o pop só recebe como operando o registrador AX
    public:
        Pop(std::string line);
};

class Push : public Semantic {
    private:
        //o push só recebe como operando o registrador AX
    public:
        Push(std::string line);
};

class Poft : public Semantic {
    private:
        //o poft não recebe nenhum operando
    public:
        Poft(std::string line);
};

class Pushf : public Semantic {
    private:
        //o pushf não recebe nenhum operando
    public:
        Pushf(std::string line);
};

//montagem

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

//declaração de variavel do tamanho de uma word, pode ser inteiro, uma constante, end de expressao, ...
class Dw : public Semantic {
    private:
        std::string name;
        int value;          //por enquanto ta só int porque é o que tem no exemplo de entrada do trabalho
    public:
        Dw(std::string line, std::string name, int value);
        std::string getName();
        int getValue();
};

//declaração de constante
class Equ : public Semantic {
    private:
        std::string name;
        std::string expression;     //essa expressão eu não sei como funciona, então fica assim por enquanto
    public:
        Equ(std::string line, std::string name, std::string expression);
        std::string getName();
        std::string getExpression();
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

/*
class Org : Semantic {

};
*/


//logicas
/*
class Or : public Semantic {
    private:
        //primeiro operando : regitrador AX
        std::string segundoOperando; //registrador AX, DX ou cte
    public:
        Or(std::string line, std::string segundoOperando);
};

class And : public Semantic {
    private:
        int opcodeAX;         //regitrador AX
        //std::string secondParameter;    //regitrador AX, DX ou cte
    public:
        And(std::string line);
        //opcode = 23 ou 25
};

class Not : public Semantic {
    private:
        int opcodeAX;         //regitrador AX
    public:
        Not(std::string line);
        //opcode = F7
};

class Xor : public Semantic {
    private:
        int opcodeAX;         //regitrador AX
        //std::string secondParameter;    //regitrador AX, DX ou cte
    public:
        Xor(std::string line);
        //opcode = 33 ou 35
};
*/

//desvio
/*
class Jmp : public Semantic {
    private:
        int end;         //endereço
    public:
        Jmp(std::string line);
        //opcode = EB
};

class JE : public Semantic {
    private:
        int end;         //endereço
    public:
        JE(std::string line);
        //opcode = 74
};

class Jnz : public Semantic {
    private:
        int end;         //endereço
    public:
        Jnz(std::string line);
        //opcode = 75
};

class Jz : public Semantic {
    private:
        int end;         //endereço
    public:
        Jz(std::string line);
        //opcode = 74
};

class Jp : public Semantic {
    private:
        int end;         //endereço
    public:
        Jp(std::string line);
        //opcode = 7A
};

class Call : public Semantic {
    private:
        int end;         //endereço
    public:
        Call(std::string line);
        //opcode = E8
};

class Int : public Semantic {
    private:
        //int cte;         //sei la oq é cte
    public:
        Int(std::string line);
        //opcode = CD
};

class Ret : public Semantic {
    private:

    public:
        Ret(std::string line);
        //opcode = C3
};
*/











#endif // SEMANTIC_H
