#include <vector>
#include <bitset>
#include "Z808Processor.h"

#define WAIT_INPUT 1

class Z808Machine
{
private:
    Z808Processor *processor;
    std::vector<Z808Byte> *memory;
public:
    Z808Machine(std::vector<Z808Byte> *memory);
    void memoryUpdate(std::vector<Z808Byte> *memory);
    int run();
    //something something
};