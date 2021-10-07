#include <vector>
#include <bitset>

            /*
            result = operator1 & operator2; //and
            result = operator1 | operator2; //or
            result = operator1 ^ operator2; //xor
            result = ~operator1; //not
            */

typedef std::bitset<16> Z808Word;   //Registradores
typedef unsigned char Z808Byte;     //Leitura da memória de instruções
typedef long int Z808Operation;     //Operaçoes dentro da VM

#define SP_MAXADDR 0xF100               //Definição do limite da pilha
#define SP_MINADDR 0xF000               //Definição do início da pilha

class Z808Processor
{
private:
    enum R                                  //Para usar de indice na tabela de registradores
    {
        //Registrador acumulador AX
        AX,
        //Registrador de dados DX
        DX,
        //Registrador de pilha SP
        SP,
        //Registrador de indice SI
        SI,
        //Registrador contador de programa IP
        IP,
        //Registrador das flags de estado SR
        SR
    };
    enum SRFlags
    {
        //Carry flag
        CF = 0,
        //Parity flag
        PF = 6,
        //Interruption flag
        IF = 7,
        //Zero flag
        ZF = 8,
        //Signal flag
        SF = 9,
        //Overflow flag
        OF = 12
    };

    enum Instruction    //Para a chamada do registrador SR
    {
        ADD,
        SUB,
        MULT,
        DIV,
        CMP,
        AND,
        NOT,
        OR,
        XOR
    };

    //Tabela de registradores
    std::vector<Z808Word> Z808Registers;

    //Flag para caso alguma instrução esteja errada
    bool errorInstruction;

    //Flag para caso aconteça um push depois do limite da pilha
    bool stackOverflow;

    //Flag para caso aconteça um pop no mínimo da pilha
    bool stackUnderflow;

    //Setar flags no registrador SR
    void setSR(Z808Operation op1, Z808Operation op2, int instruction);

public:
    Z808Processor();
    
    //Pega os registradores
    std::vector<Z808Word> getRegisters();
    //Limpa todas as flags de erro
    void clearError();
    //Checagem se a última leitura de instrução deu erro
    bool instructionError();
    //Checagem se houve estouro no maximo da pilha
    bool stackOverflowError();
    //Checagem se houve estouro no minimo da pilha
    bool stackUnderflowError();

    /**
     * Recebe a memória inteira
     * e um parâmetro para decidir se executa a leitura pelo IP ou por um índice específico
     * retorna a quantidade de bytes da operação (deslocamento)
     * SE opbytes = 0 E errorInstruction = false, CHEGOU EM HALT
     */
    int execute(std::vector<Z808Byte> memory, long int i = -1);
};