#ifndef LEXICON_SCANNER_STATUS_H
#define LEXICON_SCANNER_STATUS_H

#include <string>

#include "Dictionaries.h"
#include "Token.h"

class LexiconScannerStatus {
    private:
        bool accepted;
    protected:
        LexiconScannerStatus(bool accepted);
    public:
        bool isAccepted();
};

class SuccessStatus : public LexiconScannerStatus {
    private:
        Token * token;
    public:
        SuccessStatus(std::string token, TokenTypes tokenType, TokenNames tokenName, bool endOfLine);
        std::string getToken();
        TokenTypes getTokenType();
        TokenNames getTokenName();
        bool isEndOfLine();
        Token * getTokenObject();
};

class FailStatus : public LexiconScannerStatus {
    private:
        std::string errorMessage;
    public:
        FailStatus(std::string errorMessage);
        std::string getErrorMessage();
};

#endif /* LEXICON_SCANNER_STATUS_H */
