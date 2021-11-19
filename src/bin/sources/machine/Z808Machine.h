#ifndef Z808MACHINE_H
#define Z808MACHINE_H

#include <vector>
#include <bitset>
#include "Z808Processor.h"
#include "../GlobalSettings.h"

#define IO_T short int

#define PROGRAM_BEGIN 0

class Z808Machine
{
private:
    IO_T ioData;
    unsigned short ioAddr;
    //"0" para read
    //"1" para write
    //"2" para busca do segmento de dados
    //"3" para busca do segmento de pilha
    int ioMode;

    bool programEnd;

    Z808Processor *processor;
    std::vector<Z808Byte> *memory;

public:
    Z808Machine();
    //Será editado quando voltarmos a trabalhar com Assembler
    void memoryUpdate(std::vector<unsigned char> *memory, std::vector<unsigned char> *programBytes = nullptr);
    bool isEnd();
    void forceStop();
    //Recebe uma string contendo número, de -32768 até 32767 (int de 2 bytes)
    void setInput(std::string input);

    //Reinicia a máquina
    void resetMachine();
    //Configura o início do programa
    void setStartProgram(long, long);

    /*
        - isBySteps - executa instruções qnd nextStepRequested for true
    */
    int run(bool isBySteps);
};

#endif // Z808MACHINE_H
