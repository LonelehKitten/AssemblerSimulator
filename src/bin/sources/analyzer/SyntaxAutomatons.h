#ifndef SYNTAXAUTOMATONS_H
#define SYNTAXAUTOMATONS_H

#include <functional>
#include <string>
#include <iostream>
#include <vector>

#include "Dictionaries.h"

class SyntaxAnalyzer;

namespace SyntaxAutomatons {

    class Transition {

        private:
            AutomatonPattern automatonPattern;
            TokenTypes tokenType;
            std::function<bool(SyntaxAnalyzer *)> state;
            std::string token, pop, push;
            bool customFlag, load, id, undo;
            std::function<void(SyntaxAnalyzer *)> callback;

        public:
            Transition(AutomatonPattern automatonPattern, TokenTypes tokenTypes=TokenTypes::tNULL_TYPE);


            AutomatonPattern getAutomatonPattern();
            TokenTypes getTokenType();
            const std::function<bool(SyntaxAnalyzer *)> getState();
            std::string getToken();
            std::string getPop();
            std::string getPush();
            bool getCustomFlag();
            bool getLoad();
            bool isId();
            bool mustUndo();
            const std::function<void(SyntaxAnalyzer *)> getCallback();

            void setAutomatonPattern(AutomatonPattern automatonPattern);
            void setTokenType(TokenTypes tokenType);
            void setState(const std::function<bool(SyntaxAnalyzer *)> state);
            void setToken(std::string token);
            void setPop(std::string pop);
            void setPush(std::string push);
            void setCustomFlag(bool customFlag);
            void setLoad(bool load);
            void setId(bool id);
            void setUndo(bool undo);
            void setCallback(const std::function<void(SyntaxAnalyzer *)> callback);
    };

    bool qBegin(SyntaxAnalyzer * analyzer);
    bool q1(SyntaxAnalyzer * analyzer);
    bool q1_1(SyntaxAnalyzer * analyzer);
    bool q1_1_1(SyntaxAnalyzer * analyzer);

    bool qEnd(SyntaxAnalyzer * analyzer);
};

#endif // SYNTAXAUTOMATONS_H
