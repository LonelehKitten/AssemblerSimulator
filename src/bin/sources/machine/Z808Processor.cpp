#include "Z808Processor.h"

Z808Processor::Z808Processor()
{
    Z808Registers = std::vector<Z808Word>(6,0);
    errorInstruction = false;
}

std::vector<Z808Word> Z808Processor::getRegisters()
{
    return Z808Registers;
}

bool Z808Processor::instructionError()
{
    return errorInstruction;
}

void Z808Processor::setSR(Z808Operation op1, Z808Operation op2, bool add, bool sub, bool mult, bool this_div, bool cmp, bool this_and, bool this_not, bool this_or, bool this_xor)
{
    Z808Operation value = 0;

    if (add)
        value = op1 + op2;
    else if (sub)
        value = op1 - op2;
    else if (mult)
        value = op1 * op2;
    else if (this_div)
        value = op1 / op2;
    else if (cmp)
        value = op1 != op2;     //!= retorna 0 quando igual, seta o registrador ZF
    else if (this_and)
        value = op1 & op2;
    else if (this_not)
        value = ~op1;
    else if (this_or)
        value = op1 | op2;
    else if (this_xor)
        value = op1 ^ op2;

    Z808Word word = (Z808Word) value;

                                        //flag CF
    if (add)
        if (value & 0x00010000)
            Z808Registers[SR].set(0);
        else
            Z808Registers[SR].reset(0);
    else if (sub)
        if (op1 < 0 && op2 > 0 && !(value & 0x80000000))
            Z808Registers[SR].set(0);
        else
            Z808Registers[SR].reset(0);
    else if (mult)
        if (value & 0xFFFF0000)
            Z808Registers[SR].set(0);
        else
            Z808Registers[SR].reset(0);
    else
        Z808Registers[SR].reset(0);

                                        //flag PF
    if (mult)                           //Testa resultado da mult com ambos 32 bits
    {
        std::bitset<32> hi = (unsigned long) value;
        if ((word.count() + hi.count()) % 2 == 0)
            Z808Registers[SR].set(6);
        else
            Z808Registers[SR].reset(6);
    }
    else if (word.count() % 2 == 0)
        Z808Registers[SR].set(6);
    else
        Z808Registers[SR].reset(6);
    //if (this_div)
    //{
    //    long divMod = op1 % op2;
    //    Z808Word dMod = (Z808Word) divMod;
    //    if ((word.count() + dMod.count()) % 2 == 0)
    //        Z808Registers[SR].set(6);
    //}

                                        //flag ZF
    if (value == 0)
        Z808Registers[SR].set(8);
    else
        Z808Registers[SR].reset(8);
    if (this_div)                            //Testa se a divisao foi zero mas o resto nao foi zero, ai reseta a flag
    {
        long divMod = op1 % op2;
        if ((value == 0 && divMod != 0))
            Z808Registers[SR].reset(6);
    }

                                        //flag SF
    if (value & 0x80000000)
        Z808Registers[SR].set(9);
    else
        Z808Registers[SR].reset(9);

                                        //flag OF
    if (add || mult)
        if (value & 0x80000000)
            Z808Registers[SR].set(12);
        else
            Z808Registers[SR].reset(12);
    else if (sub)
        if (op1 < 0 && op2 > 0 && !(value & 0x80000000))
            Z808Registers[SR].set(12);
        else
            Z808Registers[SR].reset(12);
    else
        Z808Registers[SR].reset(12);
}

int Z808Processor::execute(std::vector<Z808Byte> memory, long int i)
{
    int opbytes = 0;                                        //Inicializacao
    errorInstruction = false;
    Z808Operation operator1 = 0, operator2 = 0, result = 0;

    if (i == -1)                                            //Caso decida seguir a execucao do programa
        i = Z808Registers[IP].to_ulong();
    
    if (i < -1 || i >= memory.size() || i > 0x0000FFFF)     //Caso inicialize com um indice invalido
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
            setSR(operator1, operator2, true);

        break;

        case 0xC2:          //ADD AX,DX
            //Pegando os operadores
            operator1 = (Z808Operation) Z808Registers[AX].to_ulong();
            operator2 = (Z808Operation) Z808Registers[DX].to_ulong();

            result = operator1 + operator2;             //Operacao

            Z808Registers[AX] = (Z808Word) result;      //Salvando na saida (registrador)
            Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);
            
            //Atualizando registrador de flags
            setSR(operator1, operator2, true);
            
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
        setSR(operator1, operator2, true);

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

                //Atualizando registrador de flags
                setSR(aux, operator2, false, false, false, true);   //Envia os operadores originais
            
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

                //Atualizando registrador de flags
                setSR(operator1, result, false, false, false, true);   //Envia os operadores originais

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
                setSR(operator1, operator2, false, false, true);
               
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
                setSR(operator1, operator1, false, false, true);

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
                setSR(operator1, operator1, false, true);
            
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
                setSR(operator1, operator2, false, true);

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
        setSR(operator1, operator2, false, true);
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
        setSR(operator1, operator2, false, false, false, false, true);

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
        setSR(operator1, operator2, false, false, false, false, true);
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
                setSR(operator1, operator2, false, false, false, false, false, true);
            break;
            
            case 0xC2:      //and AX, DX
                operator1 = (Z808Operation) Z808Registers[AX].to_ulong();
                operator2 = (Z808Operation) Z808Registers[DX].to_ulong();

                result = operator1 & operator2;

                Z808Registers[AX] = (Z808Word) result;      //Salvando na saida (registrador)
                Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);

                //Atualizando registrador de flags
                setSR(operator1, operator2, false, false, false, false, false, true);
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
        setSR(operator1, operator2, false, false, false, false, false, true);
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
        setSR(operator1, operator1, false, false, false, false, false, false, true);
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
                setSR(operator1, operator2, false, false, false, false, false, false, false, true);
            break;
            
            case 0xC2:      //or AX, DX
                operator1 = (Z808Operation) Z808Registers[AX].to_ulong();
                operator2 = (Z808Operation) Z808Registers[DX].to_ulong();

                result = operator1 | operator2;

                Z808Registers[AX] = (Z808Word) result;      //Salvando na saida (registrador)
                Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);
            
                //Atualizando registrador de flags
                setSR(operator1, operator2, false, false, false, false, false, false, false, true);
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
        setSR(operator1, operator2, false, false, false, false, false, false, false, true);

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
                setSR(operator1, operator2, false, false, false, false, false, false, false, false, true);
            break;
            
            case 0xC2:      //xor AX, DX
                operator1 = (Z808Operation) Z808Registers[AX].to_ulong();
                operator2 = (Z808Operation) Z808Registers[DX].to_ulong();

                result = operator1 ^ operator2;

                Z808Registers[AX] = (Z808Word) result;      //Salvando na saida (registrador)
                Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);
            
                //Atualizando registrador de flags
                setSR(operator1, operator2, false, false, false, false, false, false, false, false, true);
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
        setSR(operator1, operator2, false, false, false, false, false, false, false, false, true);

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

        if (Z808Registers[SR][ZF] == 1)
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

        if (Z808Registers[SR][ZF] != 1)
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

        if (Z808Registers[SR][SF] == 0)
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
        
        operator2 =  (Z808Operation) (Z808Registers[IP].to_ulong() + opbytes);     //Passa a posicao de retorno posterior (chamada de ret)

        Z808Registers[SP] = (Z808Word) (Z808Registers[SP].to_ulong() + 2);                  //Incremento na posicao atual da pilha
        
        memory[Z808Registers[SP].to_ulong()] = (Z808Byte) operator2;      //Push no endereco da pilha
        operator2 >>= 8;
        memory[Z808Registers[SP].to_ulong()+1] = (Z808Byte) operator2;

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

        operator1 |= memory[Z808Registers[SP].to_ulong()];                                  //Pop do topo da pilha
        operator1 <<= 8;
        operator1 |= memory[Z808Registers[SP].to_ulong()+1];

        Z808Registers[SP] = (Z808Word) (Z808Registers[SP].to_ulong() - 2);                  //Decremento na posicao atual da pilha

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
                

                operator1 |= memory[Z808Registers[SP].to_ulong()];                                  //Pop do topo da pilha
                operator1 <<= 8;
                operator1 |= memory[Z808Registers[SP].to_ulong()+1];

                Z808Registers[SP] = (Z808Word) (Z808Registers[SP].to_ulong() - 2);                  //Decremento na posicao atual da pilha

                Z808Registers[AX] = (Z808Word) operator1;       //Passa para o AX

                Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);

            break;

            case 0xC2 :     //pop DX

                operator1 |= memory[Z808Registers[SP].to_ulong()];                                  //Pop do topo da pilha
                operator1 <<= 8;
                operator1 |= memory[Z808Registers[SP].to_ulong()+1];

                Z808Registers[SP] = (Z808Word) (Z808Registers[SP].to_ulong() - 2);                  //Decremento na posicao atual da pilha

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

        operator2 |= memory[Z808Registers[SP].to_ulong()];                      //Pop do topo da pilha
        operator2 <<= 8;
        operator2 |= memory[Z808Registers[SP].to_ulong()+1];

        Z808Registers[SP] = (Z808Word) (Z808Registers[SP].to_ulong() - 2);      //Decremento na posicao atual da pilha

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

        operator1 |= memory[Z808Registers[SP].to_ulong()];                                  //Pop do topo da pilha
        operator1 <<= 8;
        operator1 |= memory[Z808Registers[SP].to_ulong()+1];

        Z808Registers[SP] = (Z808Word) (Z808Registers[SP].to_ulong() - 2);                  //Decremento na posicao atual da pilha

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

                Z808Registers[SP] = (Z808Word) (Z808Registers[SP].to_ulong() + 2);              //Incremento na posicao atual da pilha

                memory[Z808Registers[SP].to_ulong()] = (Z808Byte) operator1;                    //Push no topo da pilha
                operator1 >>= 8;
                memory[Z808Registers[SP].to_ulong()+1] = (Z808Byte) operator1;

                Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);
            break;

            case 0xC2 :     //push DX
                operator1 = (Z808Operation) Z808Registers[DX].to_ulong();

                Z808Registers[SP] = (Z808Word) (Z808Registers[SP].to_ulong() + 2);              //Incremento na posicao atual da pilha

                memory[Z808Registers[SP].to_ulong()] = (Z808Byte) operator1;                    //Push no topo da pilha
                operator1 >>= 8;
                memory[Z808Registers[SP].to_ulong()+1] = (Z808Byte) operator1;

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

        Z808Registers[SP] = (Z808Word) (Z808Registers[SP].to_ulong() + 2);              //Incremento na posicao atual da pilha

        memory[Z808Registers[SP].to_ulong()] = (Z808Byte) operator1;                    //Push no topo da pilha
        operator1 >>= 8;
        memory[Z808Registers[SP].to_ulong()+1] = (Z808Byte) operator1;

        Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);

    }
    break;

    case 0x07 :         //store reg
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
            case 0xC0:              //store AX

                operator1 |= memory[i+2];               //Pegando o opd
                operator1 <<= 8;
                operator1 |= memory[i+1];

                operator2 = (Z808Operation) Z808Registers[AX].to_ulong();   //Pegando o registrador

                memory[operator1] = (Z808Byte) operator2;
                operator2 >>= 8;
                memory[operator1+1] = (Z808Byte) operator2;

                Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);

            break;
            case 0xC2:              //store DX

                operator1 |= memory[i+2];               //Pegando o opd
                operator1 <<= 8;
                operator1 |= memory[i+1];

                operator2 = (Z808Operation) Z808Registers[DX].to_ulong();   //Pegando o registrador

                memory[operator1] = (Z808Byte) operator2;
                operator2 >>= 8;
                memory[operator1+1] = (Z808Byte) operator2;

                Z808Registers[IP] = (Z808Word) (Z808Registers[IP].to_ulong() + opbytes);

            break;
        }

    }
    break;

    case 0x12 :         //read opd
    {

    }
    break;

    case 0x08 :         //write opd
    {

    }
    break;

    default:                //Instrucao invalida
        errorInstruction = true;
    break;
    }

    return opbytes;    //Retorna o incremento do indice
}
