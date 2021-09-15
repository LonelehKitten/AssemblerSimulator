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


        std::string aux1, aux2, aux3;
        std::vector<std::string> * vaux;
        bool macroScope;

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

        void setState(const std::function<bool(SyntaxAnalyzer *)> state);



        const std::string getAux1() const;
        const std::string getAux2() const;
        const std::string getAux3() const;
        std::vector<std::string> * getVAux() const;
        bool isMacroScope() const;


        void setAux1(const std::string aux1);
        void setAux2(const std::string aux2);
        void setAux3(const std::string aux3);
        void setVAux(std::vector<std::string> * vaux);
        void setMacroScope(bool macroScope);

};

#endif // SYNTAXANALYZER_H
