#include <string>

struct Label
{
    std::string name;
    std::string value;
    int address;
    
    Label(std::string, std::string, int);
};
