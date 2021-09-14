#ifndef SYNTAXANALYZER_H
#define SYNTAXANALYZER_H

#include <string>
#include <iostream>
#include <vector>
#include <functional>
#include <stack>

#include "LexiconScanner.h"
#include "LexiconScannerStatus.h"
#include "SyntaxAutomatons.h"
#include "Token.h"

class SyntaxAnalyzer {

    private:
        std::string line;
        bool last;
        bool error;

        std::stack<std::string> * stack;
        LexiconScanner * scanner;
        LexiconScannerStatus * status;

        std::function<bool(SyntaxAnalyzer *)> state;

        std::vector<Token *> row;

        void log(std::string msg);

        void trim();

    public:
        SyntaxAnalyzer();

        void set(std::string line, bool last);

        bool init();

        void predict();

        bool check();

        void getRow();

        bool q(SyntaxAutomatons::Transition * transition);

        LexiconScanner *getScanner() const;

        void setError(bool newError);


        LexiconScannerStatus * getStatus() const;
};

#endif // SYNTAXANALYZER_H
