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
#include "Semantic.h"

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

        bool validate(SyntaxAutomatons::Transition *);

        std::string aux1, aux2, aux3;
        std::vector<std::string> * vaux;
        bool isMacroContent;
        std::stack<std::string> * macroStack;

        std::vector<Token *> * getExpression(int it, int& pointer);

        std::function<bool(SyntaxAnalyzer *)> endpoint;

    public:
        SyntaxAnalyzer();

        void set(std::string line, bool last);

        bool init();

        void predict();

        bool check();

        Semantic * getRow();

        bool q(SyntaxAutomatons::Transition * transition);

        void undoScan();

        LexiconScanner *getScanner() const;

        void setError(bool newError);

        LexiconScannerStatus * getStatus() const;

        void setState(const std::function<bool(SyntaxAnalyzer *)> state);

        void acceptMacroContent();

        std::string getErrorMessage(int lineNumber);

        const std::string getAux1() const;
        const std::string getAux2() const;
        const std::string getAux3() const;
        std::vector<std::string> * getVAux() const;
        std::function<bool (SyntaxAnalyzer *)> getEndpoint();
        std::stack<std::string> *getStack() const;
        Token * getLastToken();
        std::stack<std::string> *getMacroStack() const;
        std::vector<Token *> * getRawRow();


        void setAux1(const std::string aux1);
        void setAux2(const std::string aux2);
        void setAux3(const std::string aux3);
        void setVAux(std::vector<std::string> * vaux);
        void setEndpoint(std::function<bool(SyntaxAnalyzer *)> endpoint);

};

#endif // SYNTAXANALYZER_H
