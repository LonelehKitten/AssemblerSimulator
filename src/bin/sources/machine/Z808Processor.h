#ifndef Z808PROCESSOR_H
#define Z808PROCESSOR_H

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

#define SP_MAXADDR 0xFFFF               //Definição do limite da pilha
#define SP_BASEADDR 0xFE00               //Definição do início da pilha

enum R                                  //Para usar de indice na tabela de registradores
{
    //Registrador acumulador AX
    AX,
    //Registrador de dados DX
    DX,
    //Registrador de pilha SP
    SP,
    //Registrador de segmento de código CS
    CS,
    //Registrador de segmento de Dados DS
    DS,
    //Registrador de segmento de pilha SS
    SS,
    //Registrador de indice SI
    SI,
    //Registrador contador de programa IP
    IP,
    //Registrador das flags de estado SR
    SR
};

class Z808Processor
{
private:
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
        CMP,
        AND,
        NOT,
        OR,
        XOR
    };

    //Tabela de registradores
    std::vector<Z808Word> Z808Registers;

    //Indica se houve uma escrita na memória
    bool opStore;
    //Guarda o endereço da posição de memória em uma operação de escrita
    Z808Word storeAddr;
    //Guarda o valor da memória em uma operação de escrita
    Z808Word storeValue;

    //Indica o tipo de interrupção
    //"0" para read
    //"1" para write
    //"2" para busca do segmento de dados
    //"3" para busca do segmento de pilha
    int interruptionMode;

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
    std::vector<Z808Word> &getRegisters();
    //Pega o registrador AX - acumulador
    Z808Word getAX();
    //Pega o registrador IP - contador de programa
    Z808Word getIP();
    //Pega o registrador SR - flags
    Z808Word* getSR();
    //Inicializa o registrador IP - começo do programa
    void setCS(long main);
    //Inicializa o registrador IP - começo do programa
    void setIP(long i);
    //Verifica se houve interrupção
    bool isInterrupt(); //Z808Registers[SR][IF];
    //Verifica se a última instrução foi de escrita na memória
    bool isStore();
    //Pega a posição de memória da última instrução de escrita
    Z808Word getStoreAddr();
    //Pega o valor da memória da última instrução de escrita
    Z808Word getStoreValue();
    //Pega o tipo de interrupção
    int getInterruptionMode();
    //Reseta os valores do processador
    void reset();
    //Reseta a flag de interrupção
    void resetInterruption();
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

#endif // Z808PROCESSOR_H
