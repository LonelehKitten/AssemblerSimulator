#ifndef TOKEN_H
#define TOKEN_H

#include <string>

#include "Dictionaries.h"

class Token {
    private:
        std::string token;
        TokenTypes type;
        TokenNames name;
        bool endOfLine;

    public:
        Token(std::string token, TokenTypes type,TokenNames name, bool endOfLine);

        std::string getToken();
        TokenTypes getType() const;
        TokenNames getName() const;
        bool isEndOfLine() const;
};

#endif // TOKEN_H
