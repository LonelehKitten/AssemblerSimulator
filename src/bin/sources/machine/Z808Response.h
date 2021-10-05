#ifndef Z808RESPONSE_H
#define Z808RESPONSE_H

#include <string>
#include <bitset>
#include <utility>

typedef unsigned short int USint;

class Z808Response {
    private:
        USint ax, dx, si, ss, ds, cs, sp, pc;
        std::bitset<16> * sr;
        std::string stdout;
        bool stdin;
        std::pair<USint, std::string> * memoryWrite;

        std::string i(USint n);
        std::string bits();

    public:
        Z808Response();

        std::string toJSON();

        void setAx(USint newAx);
        void setDx(USint newDx);
        void setSi(USint newSi);
        void setSs(USint newSs);
        void setDs(USint newDs);
        void setCs(USint newCs);
        void setSp(USint newSp);
        void setPc(USint newPc);
        void setSr(std::bitset<16> * sr);
        void setStdout(const std::string newStdout);
        void setStdin(bool newStdin);
        void setMemoryWrite(std::pair<USint, std::string> * memoryWrite);
};

#endif // Z808RESPONSE_H
