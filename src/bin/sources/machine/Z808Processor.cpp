#include "Z808Processor.h"

Z808Processor::Z808Processor()
{
    Z808Registers = std::vector<Z808Word>(6,0);
    Z808Registers[SP] = SP_BASEADDR;
    clearError();
}

std::vector<Z808Word>& Z808Processor::getRegisters()
{
    return Z808Registers;
}

Z808Word Z808Processor::getAX()
{
    return Z808Registers[AX];
}

Z808Word Z808Processor::getIP()
{
    return Z808Registers[IP];
}

bool Z808Processor::isInterrupt()
{
    return Z808Registers[SR].test(IF);
}

bool Z808Processor::getInterruptionMode() 
{
    return interruptionMode;
}

void Z808Processor::resetInterruption()
{
    Z808Registers[SR].reset(IF);
}

bool Z808Processor::instructionError()
{
    return errorInstruction;
}

bool Z808Processor::stackOverflowError()
{
    return stackOverflow;
}

bool Z808Processor::stackUnderflowError()
{
    return stackUnderflow;
}

void Z808Processor::clearError()
{
    errorInstruction = false;
    stackOverflow = false;
    stackUnderflow = false;
}

void Z808Processor::setSR(Z808Operation op1, Z808Operation op2, int instruction)
{
    Z808Operation value = 0;

    if (instruction == ADD)
        value = op1 + op2;
    else if (instruction == SUB)
        value = op1 - op2;
    else if (instruction == MULT)
        value = op1 * op2;
    else if (instruction == CMP)
        value = op1 == op2;
    else if (instruction == AND)
        value = op1 & op2;
    else if (instruction == NOT)
        value = ~op1;
    else if (instruction == OR)
        value = op1 | op2;
    else if (instruction == XOR)
        value = op1 ^ op2;

    Z808Word word = (Z808Word) value, wordop1 = (Z808Word) op1, wordop2 = (Z808Word) op2;

    if (instruction == ADD)
    {
                                            //flag CF
        if (word[16] && (wordop1[15] || wordop2[15]))
            Z808Registers[SR].set(CF);
        else
            Z808Registers[SR].reset(CF);
                                            //flag PF
        if (word.count() % 2 == 0)
            Z808Registers[SR].set(PF);
        else
            Z808Registers[SR].reset(PF);
                                            //flag ZF
        if (value == 0)
            Z808Registers[SR].set(ZF);
        else
            Z808Registers[SR].reset(ZF);
                                            //flag SF
        if (word[31])
            Z808Registers[SR].set(SF);
        else
            Z808Registers[SR].reset(SF);
                                            //flag OF
        if (op1 > 0 && op2 > 0 && value < 0 || op1 < 0 && op2 < 0 && value > 0)
            Z808Registers[SR].set(OF);
        else
            Z808Registers[SR].reset(OF);
    }
    
    if (instruction == SUB)
    {
                                            //flag CF
        if (op1 < 0 && op2 > 0 && !word[31])
            Z808Registers[SR].set(CF);
        else
            Z808Registers[SR].reset(CF);
                                            //flag PF
        if (word.count() % 2 == 0)
            Z808Registers[SR].set(PF);
        else
            Z808Registers[SR].reset(PF);
                                            //flag ZF
        if (value == 0)
            Z808Registers[SR].set(ZF);
        else
            Z808Registers[SR].reset(ZF);
                                            //flag SF
        if (word[31])
            Z808Registers[SR].set(SF);
        else
            Z808Registers[SR].reset(SF);
                                            //flag OF
        if (op1 > 0 && (-op2) > 0 && value < 0 || op1 < 0 && (-op2) < 0 && value > 0)
            Z808Registers[SR].set(OF);
        else
            Z808Registers[SR].reset(OF);
    }

    if (instruction == MULT)
    {
                                            //flag CF
        Z808Registers[SR].reset(CF);
                                            //flag OF
        Z808Registers[SR].reset(OF);
    }

    if (instruction == CMP)
    {
                                            //flag ZF
        if (value)
            Z808Registers[SR].set(ZF);
        else
            Z808Registers[SR].reset(ZF);
    }

    if (instruction == AND || instruction == OR || instruction == XOR || instruction == NOT)
    {
                                            //flag CF
        Z808Registers[SR].reset(CF);
                                            //flag PF
        if (word.count() % 2 == 0)
            Z808Registers[SR].set(PF);
        else
            Z808Registers[SR].reset(PF);
                                            //flag ZF
        if (value == 0)
            Z808Registers[SR].set(ZF);
        else
            Z808Registers[SR].reset(ZF);
                                            //flag SF
        if (value & 0x00008000)
            Z808Registers[SR].set(SF);
        else
            Z808Registers[SR].reset(SF);
                                            //flag OF
        Z808Registers[SR].reset(OF);
    }
}

int Z808Processor::execute(std::vector<Z808Byte> memory, long int i)
{
    int opbytes = 0;                                        //Inicializacao
    errorInstruction = false;
    Z808Operation operator1 = 0, operator2 = 0, result = 0;

    if (i == -1)                                            //Caso decida seguir a execucao do programa
        i = Z808Registers[IP].to_ulong();
    
    if (i < -1 || i >= memory.size() || i > 0x0001FFFF)     //Caso inicialize com um indice invalido
    {
        errorInstruction = true;
        return opbytes;
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
            Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);
                
            //Atualizando registrador de flags
            setSR(operator1, operator2, ADD);

        break;

        case 0xC2:          //ADD AX,DX
            //Pegando os operadores
            operator1 = (Z808Operation) Z808Registers[AX].to_ulong();
            operator2 = (Z808Operation) Z808Registers[DX].to_ulong();

            result = operator1 + operator2;             //Operacao

            Z808Registers[AX] = (Z808Word) result;      //Salvando na saida (registrador)
            Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);
            
            //Atualizando registrador de flags
            setSR(operator1, operator2, ADD);
            
        break;
        default:
            errorInstruction = true;
        break;
        }
    }
    break;
        
    case 0x05:              //ADD AX, opd, 3 opbytes
    {
        opbytes = 3;        

        if (i+opbytes > memory.size())       //Se a instrucao nao possui bytes suficientes antes do fim da memoria
        {
            errorInstruction = true;
            break;
        }

        operator1 = (Z808Operation) Z808Registers[AX].to_ulong();
        operator2 |= memory[i+2];   //memória vai ser little endian
        operator2 <<= 8;
        operator2 |= memory[i+1];

        result = operator1 + operator2;

        Z808Registers[AX] = (Z808Word) result;      //Salvando na saida (registrador)
        Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);

        //Atualizando registrador de flags
        setSR(operator1, operator2, ADD);

    }
    break;

                            //Pra multiplicacao, declarar variaveis unsigned long int
                            //MUL SI, 2 opbytes
                            //MUL AX, 2 opbytes
                            //DIV AX, 2 opbytes
    case 0xF7:              //DIV SI, 2 opbytes
    {
        //Processamento
        opbytes = 2;               

        if (i+opbytes > memory.size())       //Se a instrucao nao possui bytes suficientes antes do fim da memoria
        {
            errorInstruction = true;
            break;
        }

        Z808Byte op = memory[i+1];

        switch(op){
            // div AX, SI
            case 0xF6:
            {
                operator1 = (Z808Operation) Z808Registers[AX].to_ulong();
                operator2 = (Z808Operation) Z808Registers[SI].to_ulong();
                
                Z808Operation aux = operator1; // guarda o AX, para n perder o valor original na 1 OP
                
                operator1 = operator1 / operator2; // AX <- AX div SI
                result = aux % operator2; // DX <- AX mod SI

                Z808Registers[AX] = (Z808Word) operator1;      //Salvando na saida (registrador)
                Z808Registers[DX] = (Z808Word) result;
                Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);
            
                break;
            }
            // div AX, AX
            case 0xF0:
            {
                operator1 = (Z808Operation) Z808Registers[AX].to_ulong();
                Z808Operation aux = operator1; // guarda o AX, para n perder o valor original na 1 OP
               
                operator1 = operator1 / operator1; // AX <- AX div AX
                result = aux % aux; // DX <- AX mod AX
                
                Z808Registers[AX] = (Z808Word) operator1;      //Salvando na saida (registrador)
                Z808Registers[DX] = (Z808Word) result;
                Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);

                break;
            }
            // mul SI
            case 0xE6:
            {
                operator1 = (Z808Operation) Z808Registers[AX].to_ulong();
                operator2 = (Z808Operation) Z808Registers[SI].to_ulong();
                
                unsigned long int highMult;
                //unsigned long int opera1 = (unsigned long int) operator1 * (unsigned long int) operator2; // AX<-AX * SI
                
                result = operator1 * operator2;
                    
                Z808Registers[AX] = (Z808Word) result;
                highMult = (unsigned long int) result;
                highMult >>= 16;
                Z808Registers[DX] = (Z808Word) highMult;
                Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);
                        
                //Atualizando registrador de flags
                setSR(operator1, operator2, MULT);
               
            break;
            }
            // mul AX
            case 0xE0:
            {
                operator1 = (Z808Operation) Z808Registers[AX].to_ulong();

                unsigned long int highMult;
                //unsigned long int opera1 = (unsigned long int)operator1 * (unsigned long int)operator1;// AX<-AX * AX

                result = operator1 * operator1;

                Z808Registers[AX] = (Z808Word) result;
                highMult = (unsigned long int) result;
                highMult >>= 16;
                Z808Registers[DX] = (Z808Word) highMult;
                Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);
                
                //Atualizando registrador de flags
                setSR(operator1, operator1, MULT);

            }
            break;

            default:
                errorInstruction = true;
            break;  
        }

    }
    break;

    case 0x2B :             //sub AX, reg
    {
        //Processamento
        opbytes = 2;               

        if (i+opbytes > memory.size())       //Se a instrucao nao possui bytes suficientes antes do fim da memoria
        {
            errorInstruction = true;
            break;
        }

        Z808Byte op = memory[i+1];

        switch(op){
            // SUB AX, AX
            case 0xC0:
            {
                operator1 = (Z808Operation) Z808Registers[AX].to_ulong();
                
                result = operator1 - operator1; // AX <- AX - AX

                Z808Registers[AX] = (Z808Word) result;      //Salvando na saida (registrador)
                Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);
                
                //Atualizando registrador de flags
                setSR(operator1, operator1, SUB);
            
                break;
            } 
            // SUB AX, DX
            case 0xC2:
                operator1 = (Z808Operation) Z808Registers[AX].to_ulong();
                operator2 = (Z808Operation) Z808Registers[DX].to_ulong();
                
                result = operator1 - operator2; // AX <- AX - DX
                
                Z808Registers[AX] = (Z808Word) result;      //Salvando na saida (registrador)
                Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);
                
                //Atualizando registrador de flags
                setSR(operator1, operator2, SUB);

                break;
        
            default:
                errorInstruction = true;
            break;  
        }           

    }
    break;

    case 0x2D :             //sub AX, opd
    {
        opbytes = 3;               

        if (i+opbytes > memory.size())       //Se a instrucao nao possui bytes suficientes antes do fim da memoria
        {
            errorInstruction = true;
            break;
        }

        operator1 = (Z808Operation) Z808Registers[AX].to_ulong();
        operator2 |= memory[i+2];   //memória vai ser little endian
        operator2 <<= 8;
        operator2 |= memory[i+1];
        
        result = operator1 - operator2; // AX <- AX - DX

        Z808Registers[AX] = (Z808Word) result;      //Salvando na saida (registrador)
        Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);
        
        //Atualizando registrador de flags
        setSR(operator1, operator2, SUB);
        //operator2 = opd

    }
    break;

    case 0x3D :         //cmp AX, opd
    {
        opbytes = 3;

        if (i+opbytes > memory.size())       //Se a instrucao nao possui bytes suficientes antes do fim da memoria
        {
            errorInstruction = true;
            break;
        }

        operator1 = (Z808Operation) Z808Registers[AX].to_ulong();
        operator2 |= memory[i+2];   //memória vai ser little endian
        operator2 <<= 8;
        operator2 |= memory[i+1];

        Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);

        //Atualizando registrador de flags
        setSR(operator1, operator2, CMP);

    }
    break;

    case 0x3B :          //cmp AX, DX
    {
        opbytes = 2;

        if (i+opbytes > memory.size())       //Se a instrucao nao possui bytes suficientes antes do fim da memoria
        {
            errorInstruction = true;
            break;
        }

        operator1 = (Z808Operation) Z808Registers[AX].to_ulong();
        operator2 = (Z808Operation) Z808Registers[DX].to_ulong();

        //resultado: se op1 = op2 zf = 1, senão zf = 0
        //o setSR faz isso
        Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);

        //Atualizando registrador de flags
        setSR(operator1, operator2, CMP);
    }
    break;

    case 0x23 :         //and AX, AX ou DX
    {
        opbytes = 2;               

        if (i+opbytes > memory.size())
        {
            errorInstruction = true;
            break;
        }

        Z808Byte op = memory[i+1];

        switch(op) {
            case 0xC0:      //and AX, AX
                operator1 = (Z808Operation) Z808Registers[AX].to_ulong();
                operator2 = (Z808Operation) Z808Registers[AX].to_ulong();

                result = operator1 & operator2;

                Z808Registers[AX] = (Z808Word) result;      //Salvando na saida (registrador)
                Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);

                //Atualizando registrador de flags
                setSR(operator1, operator2, AND);
            break;
            
            case 0xC2:      //and AX, DX
                operator1 = (Z808Operation) Z808Registers[AX].to_ulong();
                operator2 = (Z808Operation) Z808Registers[DX].to_ulong();

                result = operator1 & operator2;

                Z808Registers[AX] = (Z808Word) result;      //Salvando na saida (registrador)
                Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);

                //Atualizando registrador de flags
                setSR(operator1, operator2, AND);
            break;
        }

    }
    break;

    case 0x25 :         //and AX, opd
    {
        opbytes =  3;
        if (i+opbytes > memory.size())
        {
            errorInstruction = true;
            break;
        }

        operator1 = (Z808Operation) Z808Registers[AX].to_ulong();

        operator2 |= memory[i+1];       //Talvez precise mudar a ordem, se o mapeamento da memoria for little endian
        operator2 <<= 8;
        operator2 |= memory[i+2];

        result = operator1 & operator2;
        Z808Registers[AX] = (Z808Word) result;      //Salvando na saida (registrador)
        Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);

        //Atualizando registrador de flags
        setSR(operator1, operator2, AND);
    }
    break;

    case 0xF8 :         //not AX
    {
        opbytes = 2;               

        if (i+opbytes > memory.size())
        {
            errorInstruction = true;
            break;
        }

        operator1 = (Z808Operation) Z808Registers[AX].to_ulong();

        result = ~operator1;
        
        Z808Registers[AX] = (Z808Word) result;
        Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);

        //Atualizando registrador de flags
        setSR(operator1, operator1, NOT);
    }
    break;

    case 0x0B :         //or AX, AX ou DX
    {
        opbytes = 2;               

        if (i+opbytes > memory.size())
        {
            errorInstruction = true;
            break;
        }

        Z808Byte op = memory[i+1];

        switch(op) {
            case 0xC0:      //or AX, AX
                operator1 = (Z808Operation) Z808Registers[AX].to_ulong();
                operator2 = (Z808Operation) Z808Registers[AX].to_ulong();

                result = operator1 | operator2;

                Z808Registers[AX] = (Z808Word) result;      //Salvando na saida (registrador)
                Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);
            
                //Atualizando registrador de flags
                setSR(operator1, operator2, OR);
            break;
            
            case 0xC2:      //or AX, DX
                operator1 = (Z808Operation) Z808Registers[AX].to_ulong();
                operator2 = (Z808Operation) Z808Registers[DX].to_ulong();

                result = operator1 | operator2;

                Z808Registers[AX] = (Z808Word) result;      //Salvando na saida (registrador)
                Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);
            
                //Atualizando registrador de flags
                setSR(operator1, operator2, OR);
            break;
        }
    }
    break;

    case 0x0D :         //or AX, opd
    {
        opbytes = 3;
        if (i+opbytes > memory.size())       //Se a instrucao nao possui bytes suficientes antes do fim da memoria
        {
            errorInstruction = true;
            break;
        }

        operator1 = (Z808Operation) Z808Registers[AX].to_ulong();
        operator2 |= memory[i+2];   //memória vai ser little endian
        operator2 <<= 8;
        operator2 |= memory[i+1];

        result = operator1 | operator2;
        
        Z808Registers[AX] = (Z808Word) result;      //Salvando na saida (registrador)
        Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);

        //Atualizando registrador de flags
        setSR(operator1, operator2, OR);

    }
    break;

    case 0x33 :         //xor AX, AX ou DX
    {
        opbytes = 2;

        if (i+opbytes > memory.size())
        {
            errorInstruction = true;
            break;
        }

        Z808Byte op = memory[i+1];

        switch(op) {
            case 0xC0:      //xor AX, AX
                operator1 = (Z808Operation) Z808Registers[AX].to_ulong();
                operator2 = (Z808Operation) Z808Registers[AX].to_ulong();

                result = operator1 ^ operator2;

                Z808Registers[AX] = (Z808Word) result;      //Salvando na saida (registrador)
                Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);
            
                //Atualizando registrador de flags
                setSR(operator1, operator2, XOR);
            break;
            
            case 0xC2:      //xor AX, DX
                operator1 = (Z808Operation) Z808Registers[AX].to_ulong();
                operator2 = (Z808Operation) Z808Registers[DX].to_ulong();

                result = operator1 ^ operator2;

                Z808Registers[AX] = (Z808Word) result;      //Salvando na saida (registrador)
                Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);
            
                //Atualizando registrador de flags
                setSR(operator1, operator2, XOR);
            break;
        }
    }
    break;

    case 0x35 :         //xor AX, opd
    {
        opbytes = 3;        

        if (i+opbytes > memory.size())       //Se a instrucao nao possui bytes suficientes antes do fim da memoria
        {
            errorInstruction = true;
            break;
        }
        operator1 = (Z808Operation) Z808Registers[AX].to_ulong();
        operator2 |= memory[i+2];   //memória vai ser little endian
        operator2 <<= 8;
        operator2 |= memory[i+1];

        result = operator1 ^ operator2;

        Z808Registers[AX] = (Z808Word) result;      //Salvando na saida (registrador)
        Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);

        //Atualizando registrador de flags
        setSR(operator1, operator2, XOR);

    }
    break;

    case 0xEB :         //jmp opd
    {
        opbytes = 3;

        if (i+opbytes > memory.size())
        {
            errorInstruction = true;
            break;
        }

        operator1 |= memory[i+2];   //memória vai ser little endian
        operator1 <<= 8;
        operator1 |= memory[i+1];

        opbytes = operator1;    //Novo deslocamento

        Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);
    }
    break;

    case 0x74 :         //jz opd
    {
        opbytes = 3;

        if (i+opbytes > memory.size())
        {
            errorInstruction = true;
            break;
        }

        operator1 |= memory[i+2];   //memória vai ser little endian
        operator1 <<= 8;
        operator1 |= memory[i+1];

        if (Z808Registers[SR].test(ZF) == 1)
            opbytes = operator1;    //Novo deslocamento

        Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);
    }
    break;

    case 0x75 :         //jnz opd
    {
        opbytes = 3;

        if (i+opbytes > memory.size())
        {
            errorInstruction = true;
            break;
        }

        operator1 |= memory[i+2];       //memória vai ser little endian
        operator1 <<= 8;
        operator1 |= memory[i+1];

        if (Z808Registers[SR].test(ZF) != 1)
            opbytes = operator1;        //Novo deslocamento

        Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);
    }
    break;

    case 0x7A :         //jp opd
    {
        opbytes = 3;

        if (i+opbytes > memory.size())
        {
            errorInstruction = true;
            break;
        }

        operator1 |= memory[i+2];   //memória vai ser little endian
        operator1 <<= 8;
        operator1 |= memory[i+1];

        if (Z808Registers[SR].test(SF) == 0)
            opbytes = operator1;    //Novo deslocamento

        Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);
    }
    break;

    case 0xE8 :         //call opd
    {
        opbytes = 3;

        if (i+opbytes > memory.size())
        {
            errorInstruction = true;
            break;
        }

        operator1 |= memory[i+2];   //memória vai ser little endian
        operator1 <<= 8;
        operator1 |= memory[i+1];
                                                                                //Passa a posicao de retorno posterior (chamada de ret)
        operator2 =  (Z808Operation) (Z808Registers[IP].to_ulong() + opbytes);
                                                                            //Push no endereco da pilha
        memory[2 * Z808Registers[SP].to_ulong()] = (Z808Byte) operator2;
        operator2 >>= 8;
        memory[2 * Z808Registers[SP].to_ulong() + 1] = (Z808Byte) operator2;
                                                                            //Incremento na posicao atual da pilha
        Z808Registers[SP] = (Z808Word) (Z808Registers[SP].to_ulong() + 1);
                                                                            
        if (Z808Registers[SP].to_ulong() >= SP_MAXADDR)                     //Verifica se passou do topo da pilha
        {
            stackOverflow = true;
        }

        Z808Registers[IP] = (Z808Word) operator1;       //Jump para a posicao da chamada
    }
    break;

    case 0xEF :         //ret
    {
        opbytes = 1;               

        if (i+opbytes > memory.size())  //Essa verificação é desnecessária pois, se leu da memória e for a última posição, sempre dara igual a memory.size() e nao maior
        {
            errorInstruction = true;
            break;
        }
        
        Z808Registers[SP] = (Z808Word) (Z808Registers[SP].to_ulong() - 1);                  //Decremento na posicao atual da pilha
                                                                        //Verifica se passou do minimo da pilha
        if (Z808Registers[SP].to_ulong() < SP_BASEADDR)
        {
            stackUnderflow = true;
        }
        
        operator1 |= memory[2 * Z808Registers[SP].to_ulong()];                                  //Pop do topo da pilha
        operator1 <<= 8;
        operator1 |= memory[2 * Z808Registers[SP].to_ulong() + 1];

        Z808Registers[IP] = (Z808Word) operator1;       //Jump para a posicao

    }
    break;

    case 0xEE :         //hlt
        opbytes = 0;    //Fim de programa
    break;

    case 0x58 :         //pop reg
    {
        opbytes = 2;               

        if (i+opbytes > memory.size())
        {
            errorInstruction = true;
            break;
        }

        Z808Byte op = memory[i+1];
        
        switch(op)
        {
            case 0xC0 :     // pop AX

                Z808Registers[SP] = (Z808Word) (Z808Registers[SP].to_ulong() - 1);      //Decremento na posicao atual da pilha
                                                                       
                if (Z808Registers[SP].to_ulong() < SP_BASEADDR)                   //Verifica se passou do minimo da pilha
                {
                    stackUnderflow = true;
                }

                operator2 = Z808Registers[SP].to_ulong();
                operator2 *= 2;

                operator1 |= memory[operator2+1];                                  //Pop do topo da pilha
                operator1 <<= 8;
                operator1 |= memory[operator2];

                Z808Registers[AX] = (Z808Word) operator1;       //Passa para o AX

                Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);

            break;

            case 0xC2 :     //pop DX

                Z808Registers[SP] = (Z808Word) (Z808Registers[SP].to_ulong() - 1);                  //Decremento na posicao atual da pilha
                                                                       
                if (Z808Registers[SP].to_ulong() < SP_BASEADDR)                   //Verifica se passou do minimo da pilha
                {
                    stackUnderflow = true;
                }

                operator2 = Z808Registers[SP].to_ulong();
                operator2 *= 2;

                operator1 |= memory[operator2];                                  //Pop do topo da pilha
                operator1 <<= 8;
                operator1 |= memory[operator2+1];

                Z808Registers[DX] = (Z808Word) operator1;       //Passa para o AX

                Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);

            break;
        }
    }                                                                                                               
    break;

    case 0x59 :         //pop opd
    {
        opbytes = 3;

        if (i+opbytes > memory.size())
        {
            errorInstruction = true;
            break;
        }

        operator1 |= memory[i+2];               //Pegando o opd
        operator1 <<= 8;
        operator1 |= memory[i+1];

        Z808Registers[SP] = (Z808Word) (Z808Registers[SP].to_ulong() - 1);      //Decremento na posicao atual da pilha
                                                                       
        if (Z808Registers[SP].to_ulong() < SP_BASEADDR)                   //Verifica se passou do minimo da pilha
        {
            stackUnderflow = true;
        }

        operator2 |= memory[Z808Registers[SP].to_ulong()];                      //Pop do topo da pilha
        operator2 <<= 8;
        operator2 |= memory[Z808Registers[SP].to_ulong()+1];

        memory[operator1] = (Z808Byte) operator2;          //Salva o topo da pilha, salvo em operator2, na posicao salva em operator1
        operator2 >>= 8;
        memory[operator1+1] = (Z808Byte) operator2;

        Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);

    }
    break;

    case 0x9D :         //popf
    {
        opbytes = 1;               

        if (i+opbytes > memory.size())  //Essa verificação é desnecessária pois, se leu da memória e for a última posição, sempre dara igual a memory.size() e nao maior
        {
            errorInstruction = true;
            break;
        }

        Z808Registers[SP] = (Z808Word) (Z808Registers[SP].to_ulong() - 1);                  //Decremento na posicao atual da pilha
                                                                       
        if (Z808Registers[SP].to_ulong() < SP_BASEADDR)                   //Verifica se passou do minimo da pilha
        {
            stackUnderflow = true;
        }

        operator2 = Z808Registers[SP].to_ulong();
        operator2 *= 2;

        operator1 |= memory[operator2+1];                                  //Pop do topo da pilha
        operator1 <<= 8;
        operator1 |= memory[operator2];

        Z808Registers[SR] = (Z808Word) operator1;       //Passa para o SR

        Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);
    }
    break;

    case 0x50 :         //push reg
    {
        opbytes = 2;               

        if (i+opbytes > memory.size())
        {
            errorInstruction = true;
            break;
        }

        Z808Byte op = memory[i+1];
        
        switch(op)
        {
            case 0xC0 :     // push AX
                operator1 = (Z808Operation) Z808Registers[AX].to_ulong();
                operator2 = Z808Registers[SP].to_ulong();
                operator2 *= 2;

                memory[operator2] = (Z808Byte) operator1;                    //Push no topo da pilha
                operator1 >>= 8;
                memory[operator2+1] = (Z808Byte) operator1;

                Z808Registers[SP] = (Z808Word) (Z808Registers[SP].to_ulong() + 1);              //Incremento na posicao atual da pilha
                                                                            
                if (Z808Registers[SP].to_ulong() >= SP_MAXADDR)                     //Verifica se passou do topo da pilha
                {
                    stackOverflow = true;
                }

                Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);
            break;

            case 0xC2 :     //push DX
                operator1 = (Z808Operation) Z808Registers[DX].to_ulong();
                operator2 = Z808Registers[SP].to_ulong();
                operator2 *= 2;

                memory[operator2] = (Z808Byte) operator1;                    //Push no topo da pilha
                operator1 >>= 8;
                memory[operator2+1] = (Z808Byte) operator1;

                Z808Registers[SP] = (Z808Word) (Z808Registers[SP].to_ulong() + 1);              //Incremento na posicao atual da pilha
                                                                            
                if (Z808Registers[SP].to_ulong() >= SP_MAXADDR)                     //Verifica se passou do topo da pilha
                {
                    stackOverflow = true;
                }

                Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);
            break;
        }
    }
    break;

    case 0x9C :         //pushf
    {
        opbytes = 1;               

        if (i+opbytes > memory.size())  //Essa verificação é desnecessária pois, se leu da memória e for a última posição, sempre dara igual a memory.size() e nao maior
        {
            errorInstruction = true;
            break;
        }

        operator1 = (Z808Operation) Z808Registers[SR].to_ulong();
        operator2 = Z808Registers[SP].to_ulong();
        operator2 *= 2;

        memory[operator2] = (Z808Byte) operator1;                    //Push no topo da pilha
        operator1 >>= 8;
        memory[operator2+1] = (Z808Byte) operator1;

        Z808Registers[SP] = (Z808Word) (Z808Registers[SP].to_ulong() + 1);              //Incremento na posicao atual da pilha
                                                                            
        if (Z808Registers[SP].to_ulong() >= SP_MAXADDR)                     //Verifica se passou do topo da pilha
        {
            stackOverflow = true;
        }

        Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);

    }
    break;

    case 0xA3 :             //mov mem, AX - basicamente  store AX
    {
        opbytes = 3;               

        if (i+opbytes > memory.size())
        {
            errorInstruction = true;
            break;
        }

        operator1 |= memory[i+2];               //Pegando o opd
        operator1 <<= 8;
        operator1 |= memory[i+1];
        operator1 *= 2;

        operator2 = (Z808Operation) Z808Registers[AX].to_ulong();   //Pegando o registrador
        operator2 *= 2;

        memory[operator1] = (Z808Byte) operator2;
        operator2 >>= 8;
        memory[operator1+1] = (Z808Byte) operator2;

        Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);
    }
    break;

    case 0x8B :                 //mov AX, reg
    {
        opbytes = 2;               

        if (i+opbytes > memory.size())
        {
            errorInstruction = true;
            break;
        }

        Z808Byte op = memory[i+1];
        
        switch(op)
        {
        case 0xC2 :             //mov AX, DX
            Z808Registers[AX] = Z808Registers[DX];

            Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);

        break;

        case 0xC4:              //mov AX, SP
        //Bastante coisa pra implementar...
        }
    }
    break;
    

    break;

    case 0xCD:         //int opd
    {
        opbytes = 3;

        if (i+opbytes > memory.size())      //Se a instrucao nao possui bytes suficientes antes do fim da memoria
        {
            errorInstruction = true;
            break;
        }

        operator1 |= memory[i+2];           //pegando opd
        operator1 <<= 8;
        operator1 |= memory[i+1];

        switch(operator1)
        {
            case 1:                         //read opd - int 1
                interruptionMode = false;
                Z808Registers[SR].set(IF);    //Seta o bit para interrupcao
                Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);
            break;

            case 2:                         //write opd - int 2
                interruptionMode = true;
                Z808Registers[SR].set(IF);    //Seta o bit para interrupcao
                Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);
            break;
            
            default:
                errorInstruction = true;
            break;
        }

        //fazer a leitura do input stream
        //operator2

        //result = registers[AX];
        //registers[SR][IF].set();
        //EM MACHINE, NAO AQUI: memory[result] = input;

        //memory[operator1] = (Z808Byte) operator2;
        //operator2 >>= 8;
        //memory[operator1+1] = (Z808Byte) operator2;


        Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);
    }
    break;

    default:                //Instrucao invalida
        errorInstruction = true;
    break;
    }

    return opbytes;    //Retorna o incremento do indice
}
