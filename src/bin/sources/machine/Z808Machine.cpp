#include "Z808Machine.h"
#include "Z808Response.h"
#include "../InterfaceBus.h"
#include <iostream>
#include <cstdlib>
#include <thread>
#include <utility>

Z808Machine::Z808Machine()
{
    this->processor = new Z808Processor();
    this->memory = nullptr;

    this->programEnd = false;

    this->ioAddr = 0;
    this->ioData = 0;
    this->ioMode = false;
}

void Z808Machine::memoryUpdate(std::vector<Z808Byte> *memory, std::vector<unsigned char> *programBytes)
{
    delete this->memory;
    this->memory = memory;

    for (int i = PROGRAM_BEGIN; programBytes != nullptr && i < programBytes->size(); i++)
        this->memory->at(i) = programBytes->at(i);

    InterfaceBus::getInstance().dispatchProgramToMemory(memory);

    while(InterfaceBus::getInstance().isUpdating());
}

bool Z808Machine::isEnd()
{
    InterfaceBus::getInstance().getMutex().lock();
    bool programEnd = this->programEnd;
    InterfaceBus::getInstance().getMutex().unlock();
    return programEnd;
}

void Z808Machine::forceStop()
{
    InterfaceBus::getInstance().getMutex().lock();
    programEnd = true;
    InterfaceBus::getInstance().getMutex().unlock();
}

void Z808Machine::setInput(std::string input)
{
    ioData = std::stoi(input);

    memory->at(ioAddr) = ioData;
    ioData >>= 8;
    memory->at(ioAddr+1) = ioData;
}

int Z808Machine::run(bool isBySteps)
{

    std::vector<Z808Word> registradores;

    Z808Response *Format = new Z808Response();

    std::pair<USint, USint> * memoryStore = new std::pair<USint, USint>;

    int increment = 0;

    InterfaceBus * interfaceBus = &InterfaceBus::getInstance();

    programEnd = false;

    std::cout << "machine: before while" << std::endl;
    
    while(!isEnd())
    {
        if(!isBySteps) std::this_thread::sleep_for(interfaceBus->getClock());

        std::cout << "machine: within while: pre execute" << std::endl;
        increment = processor->execute(*memory);
        std::cout << "machine: within while: post execute" << std::endl;

        if (processor->instructionError())
        {
            std::cout << "\nERRO NA INSTRUCAO " << processor->getIP() << "\n\n\n";

            interfaceBus->dispatchLog(std::string("Erro na instrução ") + std::to_string(processor->getIP().to_ulong()), LogStatus::ERROR);
            programEnd = true;
            
            break;
        }

        if (increment == 0 && !processor->instructionError())       //Programa chegou em HALT
        {
            std::cout << "\nFIM DE PROGRAMA\n\n\n";
            
            interfaceBus->dispatchLog(std::string("Fim do programa."), LogStatus::SUCCESS);
            interfaceBus->dispatchHalt();
            programEnd = true;
            
            break;
        }

        //Verificacao de stack overflow
        if (processor->stackOverflowError())
        {
            std::cout << "\nERRO DE OVERFLOW NA PILHA\n\n\n";

            interfaceBus->dispatchLog(std::string("Erro de stack overflow durante a instrução ") + std::to_string(processor->getIP().to_ulong()), LogStatus::ERROR);
            programEnd = true;

            break;
        }

        //Verificacao de interrupcao
        if (processor->isInterrupt())
        {
            ioMode = processor->getInterruptionMode();
            ioAddr = processor->getAX().to_ulong();
        }

        registradores = processor->getRegisters();

        //************Setters do Z808Response
        Format->setAx(registradores[0].to_ulong());
        Format->setDx(registradores[1].to_ulong());
        Format->setSp(registradores[2].to_ulong());        //sp
        Format->setSi(registradores[3].to_ulong());
        Format->setPc(registradores[4].to_ulong());        //ip
        Format->setSr(&(processor->getRegisters()[5]));

        if (processor->isStore())
        {
            *memoryStore = std::make_pair<USint, USint>((USint)processor->getStoreAddr().to_ulong(), (USint)processor->getStoreValue().to_ulong());
            Format->setMemoryWrite(memoryStore);
        }

        if (processor->isInterrupt())
        {
            if (ioMode)
            {
                ioData = 0;
                ioData |= memory->at(2*ioAddr+1);
                ioData <<= 8;
                ioData |= memory->at(2*ioAddr);
                Format->setStandardOutput(std::to_string(ioData));
                Format->setStandardInput(false);
                processor->resetInterruption();
            }
            else
            {
                Format->setStandardInput(true);
                std::cout << "REQUESTED INPUT FROM USER" << std::endl;
            }
        }
        else
            Format->setStandardInput(false);

        
        
        //************End_Setters do Z808Response

        std::cout << "machine: iomode: " << (ioMode ? "write" : "false") << std::endl;
        
        std::cout << "machine: within while: pre cycle" << std::endl;
        interfaceBus->dispatchCycle(*Format, processor->isInterrupt() && !ioMode, isBySteps);
        std::cout << "machine: within while: post cycle" << std::endl;
        while(interfaceBus->isUpdating());


        std::cout << "machine: within while: post updating" << std::endl;
        
        if(interfaceBus->isInputing())
        {
            std::cout << "machine: within while: inputing 1" << std::endl;

            while(interfaceBus->isInputing());

            std::cout << "machine: within while: inputing 2" << std::endl;

            processor->resetInterruption();

            std::cout << "machine: within while: inputing 3" << std::endl;
        }

        std::cout << "machine: within while: post inputing" << std::endl;
        
        while(isBySteps && !interfaceBus->isNextStepRequested());

        std::cout << "machine: within while: post interruptions" << std::endl;

        //Exemplo de como pegar o valor numerico do registrador AX (checar indices no Z808Processor.h)
        //std::cout << "\nAX: " << std::hex << registradores[0].to_ulong();
        //std::cout << "\nDX: " << std::hex << registradores[1].to_ulong();
        //std::cout << "\nSP: " << std::hex << registradores[2].to_ulong();
        //std::cout << "\nSI: " << std::hex << registradores[3].to_ulong();
        //std::cout << "\nIP: " << std::hex << registradores[4].to_ulong();
        //std::cout << "\nSR: " << std::hex << registradores[5].to_ulong();
        //std::cout << "\nSR->cf: " << registradores[5].test(0);
        //std::cout << "\nSR->pf: " << registradores[5].test(6);
        //std::cout << "\nSR->if: " << registradores[5].test(7);
        //std::cout << "\nSR->zf: " << registradores[5].test(8);
        //std::cout << "\nSR->sf: " << registradores[5].test(9);
        //std::cout << "\nSR->of: " << registradores[5].test(12);
        //std::cout << std::endl;
    }

    return 0;
}
