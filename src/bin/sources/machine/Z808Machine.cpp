#include "Z808Machine.h"

Z808Machine::Z808Machine()
{
    Z808Registers = std::vector<Z808Word>(6,0);
    errorInstruction = false;
}

std::vector<Z808Word> Z808Machine::getRegisters()
{
    return Z808Registers;
}

bool Z808Machine::instructionError()
{
    return errorInstruction;
}

void Z808Machine::setSR(Z808Operation op1, Z808Operation op2, bool add = true)
{

    Z808Operation value = op1 + op2;
    
    if (add)                            //flag CF
        if (value > 0x7FFF)
            Z808Registers[SR].set(0);
        else
            Z808Registers[SR].reset(0);
    else
        if (value < 0x8000)
            Z808Registers[SR].set(0);
        else
            Z808Registers[SR].reset(0);

    if (value)                          //flag PF
        Z808Registers[SR].set(6);
    else
        Z808Registers[SR].reset(6);

    if (value == 0)                     //flag ZF
        Z808Registers[SR].set(8);
    else
        Z808Registers[SR].reset(8);
    
    if (value & 0x80000000)             //flag SF
        Z808Registers[SR].set(9);
    else
        Z808Registers[SR].reset(9);

    if (value)                          //flag OF
        Z808Registers[SR].set(12);
    else
        Z808Registers[SR].reset(12);
}

int Z808Machine::execute(std::vector<Z808Byte> memory, long int i = -1)
{
    int opbytes = 0;                                        //Inicializacao
    errorInstruction = false;
    Z808Operation operator1 = 0, operator2 = 0, result = 0;

    if (i == -1)                                            //Caso decida seguir a execucao do programa
        i = Z808Registers[IP].to_ulong();
    
    if (i < -1 || i >= memory.size() || i > 0x0000FFFF)     //Caso inicialize com um indice invalido
    {
        errorInstruction = true;
        return errorInstruction;
    }

    Z808Byte instruction = memory[i];

    switch(instruction)
    {
    case 0x03:              //ADD, 2 opbytes
    {
        opbytes = 2;               

        if (i+opbytes > memory.size())       //Se a instrucao nao possui bytes suficientes antes do fim da memoria
        {
            errorInstruction = true;
            break;
        }

        Z808Byte op = memory[i+1];

        switch(op)
        {
        case 0xC0:          //ADD AX,AX
            //Pegando os operadores
            operator1 = (Z808Operation) Z808Registers[AX].to_ulong();
            operator2 = (Z808Operation) Z808Registers[AX].to_ulong();

            result = operator1 + operator2;             //Operacao

            Z808Registers[AX] = (Z808Word) result;      //Salvando na saida (registrador)
            
            //Atualizando registrador de flags
            setSR(operator1, operator2);

        break;

        case 0xC2:          //ADD AX,DX
            
        break;
        default:
            errorInstruction = true;
        break;
        }
    }
    break;
        
    case 0x05:              //ADD AX, opd
    {
        //Processamento

    }
    break;

    case 0xF7:              //DIV, 2 opbytes
    {
        //Processamento

    }
    break;

        //Continuar com as instrucoes validas

    default:                //Instrucao invalida
        errorInstruction = true;
    break;
    }

    return opbytes;    //Retorna o incremento do indice
}