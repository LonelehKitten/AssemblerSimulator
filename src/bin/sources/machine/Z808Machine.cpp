#include "Z808Machine.h"
#include "Z808Response.h"
#include "../InterfaceBus.h"
#include <iostream>
#include <cstdlib>

Z808Machine::Z808Machine()
{
    this->processor = new Z808Processor();
    this->memory = nullptr;
    this->programEnd = false;
}

void Z808Machine::memoryUpdate(std::vector<Z808Byte> *memory, std::vector<unsigned char> *programBytes)
{
    delete this->memory;
    //this->memory = memory;        //Será editado quando voltarmos a trabalhar com Assembler
    this->memory = programBytes;
    this->memory.insert(this->memory.end(), memory.begin(), memory.end());
}

int Z808Machine::run()
{
    std::vector<Z808Word> registradores;

    Z808Response *Format = new Z808Response();

    int increment = 0;

    //"false" para read
    //"true" para write
    bool ioMode = false;
    unsigned short ioAddr = 0;

    InterfaceBus * interfaceBus = &InterfaceBus::getInstance();

    while(!isEnd())
    {
        increment = processor->execute(*memory);

        if (processor->instructionError())
        {
            std::cout << "\nERRO NA INSTRUCAO " << processor->getIP() << "\n\n\n";

            //dispatchLog Error
            programEnd = true;
            
            break;
        }

        if (increment == 0 && !processor->instructionError())       //Programa chegou em HALT
        {
            std::cout << "\nFIM DE PROGRAMA\n\n\n";
            
            programEnd = true;
            //dispatchHalt
            
            break;
        }

        //Verificacao de stack overflow
        if (processor->stackOverflowError())
        {
            std::cout << "\nERRO DE OVERFLOW NA PILHA\n\n\n";

            programEnd = true;
            //dispatchLog Error

            break;
        }

        //Verificacao de interrupcao
        if (processor->isInterrupt())
        {
            ioMode = process->getInterruptionKind();
            ioAddr = process->getAX().to_ulong();
            if (ioMode) //modo write
                process->resetInterruption();
        }

        //************Setters do Z808Response

        //************Setters do Z808Response

        //dispatchCycle ioMode e o resto
        interfaceBus->dispatchCycle(processor->get, !ioMode);
        //Mandar processor->getRegisters(); pro Response

        while(interfaceBus->isUpdating());

        if(interfaceBus->isInputing())
        {
            while(interfaceBus->isInputing());
            //colocaOInputNaMemoria();
        }

        registradores = processor->getRegisters();

        //Exemplo de como pegar o valor numerico do registrador AX (checar indices no Z808Processor.h)
        std::cout << "\nAX: " << std::hex << registradores[0].to_ulong();
        std::cout << "\nDX: " << std::hex << registradores[1].to_ulong();
        std::cout << "\nSP: " << std::hex << registradores[2].to_ulong();
        std::cout << "\nSI: " << std::hex << registradores[3].to_ulong();
        std::cout << "\nIP: " << std::hex << registradores[4].to_ulong();
        //std::cout << "\nSR: " << std::hex << registradores[5].to_ulong();
        std::cout << "\nSR->cf: " << registradores[5].test(0);
        std::cout << "\nSR->pf: " << registradores[5].test(6);
        std::cout << "\nSR->if: " << registradores[5].test(7);
        std::cout << "\nSR->zf: " << registradores[5].test(8);
        std::cout << "\nSR->sf: " << registradores[5].test(9);
        std::cout << "\nSR->of: " << registradores[5].test(12);
        std::cout << std::endl;

        system("pause");    //Desculpa mas nao eh pra rodar no linux
    }

    return 0;
}