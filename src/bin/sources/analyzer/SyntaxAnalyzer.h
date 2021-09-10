#ifndef SYNTAXANALYZER_H
#define SYNTAXANALYZER_H

#include <string>
#include <iostream>
#include <vector>
#include <functional>

#include "LexiconScanner.h"

class SyntaxAnalyzer {

    private:
        std::string line;
        bool last;
        bool error;

        LexiconScanner * scanner;

        std::function<bool(SyntaxAnalyzer *)> state;

        std::vector<std::string> row;

        void log(std::string msg);

        void predict(char c);

    public:
        SyntaxAnalyzer();

        void set(std::string line, bool last);

        bool init();

        void predict();

        bool check();

        void getRow();
};

#endif // SYNTAXANALYZER_H
