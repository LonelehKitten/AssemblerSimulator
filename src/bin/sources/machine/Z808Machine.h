#include <vector>
#include <bitset>

            /*
            result = operator1 & operator2; //and
            result = operator1 | operator2; //or
            result = operator1 ^ operator2; //xor
            result = ~operator1; //not
            */

typedef std::bitset<16> Z808Word; //Registradores
typedef unsigned char Z808Byte; //Leitura da memória de instruções
typedef long int Z808Operation; //Operaçoes dentro da VM

class Z808Machine
{
private:
    std::vector<Z808Word> Z808Registers;    //Tabela de registradores
    enum R                                  //Para usar de indice na tabela de registradores
    {
        AX,
        DX,
        SP,
        SI,
        IP,
        SR
    };
    enum sr
    {
        CF = 0,
        PF = 6,
        IF = 7,
        ZF = 8,
        SF = 9,
        OF = 12
    };

    //Flag para caso alguma instrução esteja errada
    bool errorInstruction;
    //Setar flags no registrador SR
    //Se for operacao de subtracao, enviar o terceiro parametro como false
    void setSR(Z808Operation op1, Z808Operation op2, bool add = false, bool sub = false, bool mult = false, bool this_div = false, bool cmp = false, bool this_and = false, bool this_not = false, bool this_or = false, bool this_xor = false, bool popf = false);

public:
    Z808Machine();

    std::vector<Z808Word> getRegisters();
    bool instructionError();                //Checagem se a última leitura de instrução deu erro

    /**
     * Recebe a memória inteira
     * e um parâmetro para decidir se executa a leitura pelo IP ou por um índice específico
     * retorna a quantidade de bytes da operação (deslocamento)
     * SE opbytes = 0 E errorInstruction = false, CHEGOU EM HALT
     */
    int execute(std::vector<Z808Byte> memory, long int i = -1);
};