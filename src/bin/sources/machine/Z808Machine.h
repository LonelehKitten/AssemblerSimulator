#include <vector>
#include <bitset>
#include "Z808Processor.h"

#define IO_T short int

#define PROGRAM_BEGIN 0

class Z808Machine
{
private:
    IO_T ioData;
    unsigned short ioAddr;
    //"false" para read
    //"true" para write
    bool ioMode = false;

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

    /*
        - isBySteps - executa instruções qnd nextStepRequested for true
    */
    int run(bool isBySteps);
};