#include <vector>
#include <bitset>

typedef std::bitset<16> Z808Word;
typedef unsigned char Z808Byte;
typedef long int Z808Operation;

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

    //Flag para caso alguma instrução esteja errada
    bool errorInstruction;
    //Setar flags no registrador SR
    //Se for operacao de subtracao, enviar o terceiro parametro como false
    void setSR(Z808Operation op1, Z808Operation op2, bool add = true);

public:
    Z808Machine();

    std::vector<Z808Word> getRegisters();
    bool instructionError();                //Checagem se a última leitura de instrução deu erro

    /**
     * Recebe a memória inteira
     * e um parâmetro para decidir se executa a leitura pelo IP ou por um índice específico
     * retorna a quantidade de bytes da operação (deslocamento)
     */
    int execute(std::vector<Z808Byte> memory, long int i = -1);
};