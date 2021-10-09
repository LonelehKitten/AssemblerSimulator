#include "Z808Machine.h"
#include <iostream>
#include <cstdlib>

Z808Machine::Z808Machine(std::vector<Z808Byte> *memory)
{
    this->processor = new Z808Processor();
    this->memory = memory;
}

void Z808Machine::memoryUpdate(std::vector<Z808Byte> *memory)
{
    delete this->memory;
    this->memory = memory;
}

int Z808Machine::run()
{
    std::vector<Z808Word> registradores;

    int increment = 0;

    while(WAIT_INPUT)
    {
        increment = processor->execute(*memory);

        if (processor->instructionError())
        {
            std::cout << "\nERRO NA INSTRUCAO " << processor->getRegisters()[4] << "\n\n\n";
            break;
        }

        if (increment == 0 && !processor->instructionError())
        {
            std::cout << "\nFIM DE PROGRAMA\n\n\n";
            break;
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