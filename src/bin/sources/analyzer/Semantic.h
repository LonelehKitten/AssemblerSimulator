#ifndef SEMANTIC_H
#define SEMANTIC_H

#include <string>
/*
 * tipo e linha
 * operador / parametros
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
        Semantic(std::string line);
    public:
    // ========== ONLY FOR TEST
        Semantic(std::string line, Instruction type);
    // ==========

    Instruction getType();
    std::string getLine();
};

//aritmeticas

class Add : public Semantic {
    private:
        int opcodeAX;        //regitrador AX
        //std::string secondParameter;    //regitrador AX, DX ou cte
    public:
        Add(std::string line);
        //opcode = 03 ou 05
};

class Sub : public Semantic {
    private:
        int opcodeAX;         //regitrador AX
        //std::string secondParameter;    //regitrador AX, DX ou cte
    public:
        Sub(std::string line);
        //opcode = 2B ou 2D
};

class Div : public Semantic {
    private:
        int opcodeRegister; //registrador SI ou AX
    public:
        Div(std::string line);
        //opcode = F7
};

class Mul : public Semantic {
    private:
        int opcodeRegister; //registrador SI ou AX
    public:
        Mul(std::string line);
        //opcode = F7
};

class Cmp : public Semantic {
    private:
        int opcodeAX;         //regitrador AX
        //std::string secondParameter;    //regitrador DX ou cte
    public:
        Cmp(std::string line);
        //opcode = 3D ou C2
};

//logicas

class Or : public Semantic {
    private:
        int opcodeAX;         //regitrador AX
        //std::string secondParameter;    //regitrador AX, DX ou cte
    public:
        Or(std::string line);
        //opcode = 0B ou 0D
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

//desvio

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










#endif // SEMANTIC_H
