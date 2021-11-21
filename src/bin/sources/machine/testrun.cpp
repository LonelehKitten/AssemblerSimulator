/*
-----PARA EXECUTAR:
cd src
cd bin
cd sources
cd machine

g++ Z808Processor.cpp testrun.cpp -o run 
run.exe
*/

/** Minhas análises / sugestões para correção
 * 1 FEITO. Faltou um break na linha 1374 de Z808Processor.cpp
 * 2 FEITO. Nome de segmento (o símbolo) deve ser classificado como constante para a instrução mov ax, cte
 * 3 FEITO. Variável em um segmento deve receber segmentCounter / 2 no basicoAssemblerStep1, pra poder 
 * receber um endereço de palavra (1 posição na memória de 64K endereços, 2 bytes na memória de 128K 
 * índices)
 * 4 FEITO. Retirar o deslocamento com o registrador CS nos jumps em Z808Processor.cpp, pois a máquina já inicializa em referência ao CS
*/

#include <iostream>
#include "Z808Processor.h"

#define STARTPROGRAM 4
#define memory programa5

int main()
{
    std::vector<Z808Byte> programa1, programa2, programa3, programa4, programa5;

    /*
    * XOR AX, 3840
    * NOT AX
    * OR AX, AX
    * AND AX, AX
    * hlt
    *
    */
    programa1 = std::vector<Z808Byte>(10,0);

    programa1[0] = 0x35; //opcode do xor ax, opd com opd = 3840, 0x0F00
    programa1[1] = 0x00; //Byte da direita do numero 3840
    programa1[2] = 0x0F; //Byte da direita do numero 3840
    programa1[3] = 0xF8; // NOT AX
    programa1[4] = 0xC0;
    programa1[5] = 0x0B; // OR AX, AX
    programa1[6] = 0xC0;
    programa1[7] = 0x23; // AND AX, AX
    programa1[8] = 0xC0;
    programa1[9] = 0xEE; // hlt


    /** Programa exemplo (criado por mim, nao faz nada demais, apenas testa umas contas, uns jumps e dps para):
     * ADD AX, 1022
     * ADD AX, AX
     * SUB AX, 511
     * SUB AX, 511
     * JZ 17
     * SUB AX, 511
     * JMP 2
     * INT 2
     * HLT
     */
    
    //Exemplo da instrucao ADD AX, opd com opd = 1022, 0x03FE
    programa2.push_back(0x05);     //Opcode da instrucao ADD AX, opd
    programa2.push_back(0xFE);     //Byte da direita do numero 1022
    programa2.push_back(0x03);     //Byte da esquerda do numero 1022
    //Exemplo da instrucao ADD AX, AX
    programa2.push_back(0x03);     //Opcode da instrucao ADD AX, reg
    programa2.push_back(0xC0);     //Opcode do registrador AX
    //Exemplo da instrucao SUB AX, opd com opd = 511, 0x01FF
    programa2.push_back(0x2D);     //Opcode da instrucao SUB AX, opd
    programa2.push_back(0xFF);     //Byte da direita do numero 511
    programa2.push_back(0x01);     //Byte da esquerda do numero 511
    //Ou, algo mais arbitrario, ainda com opd = 511
    short int numero_instruction = 0x01FF;  //16 bits, 2 bytes, 4 hexadecimais
    programa2.push_back(0x2D);                 //Opcode da instrucao SUB AX, opd
    programa2.push_back(numero_instruction);   //passando 8 bits, byte mais a direita
    numero_instruction = numero_instruction >> 8; //Deslocamento 8 bits pra direita
    programa2.push_back(numero_instruction);   //passando 8 bits, byte que antes estavam mais a esquerda
    //Exemplo da instrucao JZ 09
    programa2.push_back(0x74);     //Opcode da instrucao jz opd
    programa2.push_back(0x09);     //Byte da direita do numero 9
    programa2.push_back(0x00);     //Byte da esquerda do numero 9
    //Repetindo SUB AX, opd com opd = 511
    programa2.push_back(0x2D);
    programa2.push_back(0xFF);
    programa2.push_back(0x01);
    //Exemplo da instrucao JMP -14, pula para o indice 03
    programa2.push_back(0xEB);     //Opcode da instrucao jmp opd
    programa2.push_back(0xF2);
    programa2.push_back(0xFF);
    //Exemplo da instrucao INT 2
    programa2.push_back(0xCD);     //Opcode da instrucao int opd
    programa2.push_back(0x02);
    programa2.push_back(0x00);
    //Exemplo da instrucao hlt
    programa2.push_back(0xEE);     //Opcode da instrucao hlt


    // Programa que funciona:
    
    // program SEGMENT
    // ASSUME CS: program
    // ASSUME DS: program
    // main:
    // add ax, 100
    // loop1:
    // sub ax, 10
    // jnz loop1
    // program ENDS
    // END main

    programa3.push_back(0x05);
    programa3.push_back(0x64);
    programa3.push_back(0x00);
    programa3.push_back(0x2D);
    programa3.push_back(0x0A);
    programa3.push_back(0x00);
    programa3.push_back(0x75);
    programa3.push_back(0xFD);
    programa3.push_back(0xFF);
    programa3.push_back(0xEE);

    // Programa que dá erro na instrução 10

    // data SEGMENT
    //     max EQU 10
    //     unit2 DW 1
    //     unit DW 1
    // data ENDS
    // program SEGMENT
    // ASSUME CS: program
    // ASSUME DS: data
    // main:
    // mov ax, data
    // mov ds, ax
    // add ax, max
    // loop1:
    // sub ax, unit
    // jnz loop1
    // program ENDS
    // END main

    programa4.push_back(0x01);
    programa4.push_back(0x00);
    programa4.push_back(0x01);
    programa4.push_back(0x00);

    programa4.push_back(0xB8);
    programa4.push_back(0x00);
    programa4.push_back(0x00);

    programa4.push_back(0x8E);
    programa4.push_back(0xD8);
    
    programa4.push_back(0x05);
    programa4.push_back(0x0A);
    programa4.push_back(0x00);
    
    programa4.push_back(0x2D);
    programa4.push_back(0x01);
    programa4.push_back(0x00);
    
    programa4.push_back(0x75);
    programa4.push_back(0xFD);
    programa4.push_back(0xFF);

    programa4.push_back(0xEE);


    // Programa que dá crash na etapa 2
    /*
    Dados SEGMENT 
        Mem1 DW ? 
        Mem2 DW ?
    Dados ENDS 
    Codigo SEGMENT 
    ASSUME CS:Codigo
    ASSUME DS:Dados
    Inicio:
    mov AX, Dados
    mov DS, AX
    mov AX, Mem2
    mov DX, AX 
    mov AX, Mem1
    add AX, DX
    mov Mem1, AX
    Codigo ENDS
    END Inicio
    */

    programa5.push_back(0x00);
    programa5.push_back(0x00);
    programa5.push_back(0x00);
    programa5.push_back(0x00);

    programa5.push_back(0xB8);
    programa5.push_back(0x00);
    programa5.push_back(0x00);

    programa5.push_back(0x8E);
    programa5.push_back(0xD8);

    programa5.push_back(0xA1);
    programa5.push_back(0x01);
    programa5.push_back(0x00);

    programa5.push_back(0x8B);
    programa5.push_back(0xD0);
    
    programa5.push_back(0xA1);
    programa5.push_back(0x00);
    programa5.push_back(0x00);

    programa5.push_back(0x03);
    programa5.push_back(0xC2);
    
    programa5.push_back(0xA3);
    programa5.push_back(0x00);
    programa5.push_back(0x00);

    programa5.push_back(0xEE);
    
    Z808Processor processor;

    processor.setCS(0);
    processor.setIP(STARTPROGRAM);
    //machine.memoryUpdate(&memory);
    //machine.run(false);

    int ioMode, ioAddr, ioData;

    int choice;
    std::vector<Z808Word> registradores;
    short incremento = 0;

    for (short i = STARTPROGRAM; i <= memory.size(); )
    {
        
        //Todos os cout e cin sao para testes, depois no lugar serao feitas a conexao da interface
        //Os cout e cin comentados estao funcionando, se quiser usar pra testar eh so descomentar

        if (i == memory.size())
        {
            std::cout << "FIM DA MEMORIA!!\n";
            break;
        }

        // std::cout << "\n0 para executar o programa a partir do IP\n1 para escolher o indice de uma instrucao\nPosicao da instrucao atual: " << std::dec << i << "\nDigite: ";

        system("pause");    //Desculpa mas nao eh pra rodar no linux

        // std::cin >> choice;
        // if (choice)
        // {
        //     std::cout << "Digite o indice: ";
        //     std::cin >> choice;
        //     incremento = processor.execute(memory,choice);
        //     i = choice;
        // }
        // else
        std::cout << "Instrucao executada: " << std::hex << i << std::endl;
        incremento = processor.execute(memory);
        std::cout << "Deslocamento para somar com i: " << std::dec << incremento << std::endl;
        //std::cout << "Deu erro? " << processor.instructionError() ? "true" : "false" << std::endl;
        
        i += incremento;
        
        //std::cout << "Proxima instrucao (para comparar com IP): " << std::hex << i << std::endl;

        if (processor.instructionError())
        {
            std::cout << "\nERRO NA INSTRUCAO " << i << "\n\n\n";
            break;
        }

        if (incremento == 0 && !processor.instructionError())
        {
            std::cout << "\nFIM DE PROGRAMA\n\n\n";
            break;
        }

        //Verificacao de interrupcao
        if (processor.isInterrupt())
        {
            ioMode = processor.getInterruptionMode();
            ioAddr = processor.getAX().to_ulong();
        }

        if (processor.isInterrupt())
        {
            if (ioMode == 1)
            {
                ioData = 0;
                ioData |= memory[2*ioAddr+1];
                ioData <<= 8;
                ioData |= memory[2*ioAddr];
                processor.resetInterruption();
                std::cout << "WRITING NUMBER \"" << ioData << "\" AT ADDRESS " << processor.getAX().to_ulong() << " !\n";
            }
            else if (ioMode == 0)
            {
                std::cout << "REQUESTED INPUT FROM USER" << std::endl;
            }
        }

        registradores = processor.getRegisters();

        //IMPLEMENTAR LOOP DE PRINTS DOS VALORES DE CADA REGISTRADOR
        //Lembrando que os registradores sao bitsets
        //Metodos pra trabalhar com a classe:
        //https://www.cplusplus.com/reference/bitset/bitset/

        //Exemplo de como pegar o valor numerico do registrador AX (checar indices no Z808Processor.h)
        std::cout << "\nAX: " << std::hex << registradores[R::AX].to_ulong();
        std::cout << "\nDX: " << std::hex << registradores[R::DX].to_ulong();
        std::cout << "\nCS: " << std::hex << registradores[R::CS].to_ulong();
        std::cout << "\nDS: " << std::hex << registradores[R::DS].to_ulong();
        std::cout << "\nSS: " << std::hex << registradores[R::SS].to_ulong();
        std::cout << "\nSP: " << std::hex << registradores[R::SP].to_ulong();
        std::cout << "\nSI: " << std::hex << registradores[R::SI].to_ulong();
        std::cout << "\nIP: " << std::hex << registradores[R::IP].to_ulong();
        //std::cout << "\nSR: " << std::hex << registradores[5].to_ulong();
        std::cout << "\nSR->cf: " << registradores[R::SR].test(0);
        std::cout << "\nSR->pf: " << registradores[R::SR].test(6);
        std::cout << "\nSR->if: " << registradores[R::SR].test(7);
        std::cout << "\nSR->zf: " << registradores[R::SR].test(8);
        std::cout << "\nSR->sf: " << registradores[R::SR].test(9);
        std::cout << "\nSR->of: " << registradores[R::SR].test(12);
        std::cout << std::endl;
        //VER COM O PESSOAL DO FRONT OUTRAS FORMAS DE VISUALIZACAO DOS REGISTRADORES

        //No codigo final, em vez de prints as saidas serao convertidas pra interface

        //std::cout << "\nDeseja parar a execucao do programa?\n0 para sim\n1 para nao\nDigite: ";
        //std::cin >> choice;
        //if (!choice)
        //    break;
    }


    return 0;
}