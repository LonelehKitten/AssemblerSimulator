#include "Token.h"

Token::Token(std::string token, TokenTypes type,TokenNames name, bool endOfLine) :
    token(token),
    type(type),
    name(name),
    endOfLine(endOfLine)
{}

std::string Token::getToken()
{
    return token;
}

TokenTypes Token::getType() const
{
    return type;
}

TokenNames Token::getName() const
{
    return name;
}

bool Token::isEndOfLine() const
{
    return endOfLine;
}
