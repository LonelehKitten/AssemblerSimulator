#include <vector>
#include <bitset>
#include "Z808Processor.h"

#define END_PROGRAM 1

class Z808Machine
{
private:
    bool programEnd;
    Z808Processor *processor;
    std::vector<Z808Byte> *memory;
public:
    Z808Machine();
    void memoryUpdate(std::vector<unsigned char> *memory, std::vector<unsigned char> *programBytes = nullptr);
    bool isEnd();
    void setEnd();

    /*
        - isBySteps - executa instruções qnd nextStepRequested for true
    */
    
    int run(/* vector<byte|Z808Byte> bytecode */);
};