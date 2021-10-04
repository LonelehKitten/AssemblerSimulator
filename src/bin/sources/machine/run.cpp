#include <iostream>
#include "Z808Machine.h"

int main()
{
    Z808Machine processor;

    std::vector<Z808Byte> memory;

    //INSERIR DENTRO DO VETOR DE MEMORIA TODAS AS INSTRUCOES DE UM PROGRAMA EXEMPLO
    //FAZER DA FORMA QUE ACHAR MELHOR

    /** Programa exemplo (criado por mim, nao faz nada demais, apenas testa umas contas, uns jumps e dps para):
     * ADD AX, 1022
     * ADD AX, AX
     * SUB AX, 511
     * SUB AX, 511
     * JZ 17
     * SUB AX, 511
     * JMP 2
     * HLT
     */
    //Exemplo da instrucao ADD AX, opd com opd = 1022, 0x03FE
    memory.push_back(0x05);     //Opcode da instrucao ADD AX, opd
    memory.push_back(0xFE);     //Byte da direita do numero 1022
    memory.push_back(0x03);     //Byte da esquerda do numero 1022
    //Exemplo da instrucao ADD AX, AX
    memory.push_back(0x03);     //Opcode da instrucao ADD AX, reg
    memory.push_back(0xC0);     //Opcode do registrador AX
    //Exemplo da instrucao SUB AX, opd com opd = 511, 0x01FF
    memory.push_back(0x2D);     //Opcode da instrucao SUB AX, opd
    memory.push_back(0xFF);     //Byte da direita do numero 511
    memory.push_back(0x01);     //Byte da esquerda do numero 511
    //Ou, algo mais arbitrario, ainda com opd = 511
    short int numero_instruction = 0x01FF;  //16 bits, 2 bytes, 4 hexadecimais
    memory.push_back(0x2D);                 //Opcode da instrucao SUB AX, opd
    memory.push_back(numero_instruction);   //passando 8 bits, byte mais a direita
    numero_instruction = numero_instruction >> 8; //Deslocamento 8 bits pra direita
    memory.push_back(numero_instruction);   //passando 8 bits, byte que antes estavam mais a esquerda
    //Exemplo da instrucao JZ 0x0011
    memory.push_back(0x74);     //Opcode da instrucao jz opd
    memory.push_back(0x14);     //Byte da direita do numero 11
    memory.push_back(0x00);     //Byte da esquerda do numero 11
    //Repetindo SUB AX, opd com opd = 511
    memory.push_back(0x2D);
    memory.push_back(0xFF);
    memory.push_back(0x01);
    //Exemplo da instrucao JMP 02, pula para o indice 03
    memory.push_back(0xEB);     //Opcode da instrucao jmp opd
    memory.push_back(0x03);
    memory.push_back(0x00);
    //Exemplo da instrucao hlt
    memory.push_back(0xEE);     //Opcode da instrucao hlt

    //CRIEM OUTROS PROGRAMAS!!!!!!!!!!!!!!!

    int choice;
    std::vector<Z808Word> registradores;
    int incremento = 0;

    for (int i = 0; i <= memory.size();)
    {
        
        //Todos os cout e cin sao para testes, depois no lugar serao feitas a conexao da interface
        //Os cout e cin comentados estao funcionando, se quiser usar pra testar eh so descomentar

        if (i == memory.size())
        {
            std::cout << "FIM DA MEMORIA!!\n";
            break;
        }

        std::cout << "\n0 para executar o programa a partir do IP\n1 para escolher o indice de uma instrucao\nPosicao da instrucao atual: " << std::dec << i << "\nDigite: ";
        std::cin >> choice;
        if (choice)
        {
            std::cout << "Digite o indice: ";
            std::cin >> choice;
            incremento = processor.execute(memory,choice);
            i = choice;
        }
        else
            incremento = processor.execute(memory);
        //std::cout << "Instrucao executada: " << std::hex << (int)memory[i] << std::endl;
        //std::cout << "Deslocamento para somar com i: " << std::dec << incremento << std::endl;
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

        registradores = processor.getRegisters();

        //IMPLEMENTAR LOOP DE PRINTS DOS VALORES DE CADA REGISTRADOR
        //Lembrando que os registradores sao bitsets
        //Metodos pra trabalhar com a classe:
        //https://www.cplusplus.com/reference/bitset/bitset/

        //Exemplo de como pegar o valor numerico do registrador AX (checar indices no Z808Machine.h)
        std::cout << "\nAX: " << std::hex << registradores[0].to_ulong();
        std::cout << "\nSR: " << std::hex << registradores[5].to_ulong();
        std::cout << "\nIP: " << std::hex << registradores[4].to_ulong() << std::endl;
        //VER COM O PESSOAL DO FRONT OUTRAS FORMAS DE VISUALIZACAO DOS REGISTRADORES

        //No codigo final, em vez de prints as saidas serao convertidas pra interface

        system("pause");    //Desculpa mas nao eh pra rodar no linux

        //std::cout << "\nDeseja parar a execucao do programa?\n0 para sim\n1 para nao\nDigite: ";
        //std::cin >> choice;
        //if (!choice)
        //    break;
    }

    return 0;
}