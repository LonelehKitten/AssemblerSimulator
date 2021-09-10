#ifndef LEXICON_SCANNER_STATUS_H
#define LEXICON_SCANNER_STATUS_H

#include <string>

#include "Dictionaries.h"

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
        std::string token;
        TokenTypes tokenType;
        bool endOfLine;
    public:
        SuccessStatus(std::string token, TokenTypes tokenType, bool endOfLine);
        std::string getToken();
        TokenTypes getTokenType();
        bool isEndOfLine();
};

class FailStatus : public LexiconScannerStatus {
    private:
        std::string errorMessage;
    public:
        FailStatus(std::string errorMessage);
        std::string getErrorMessage();
};

#endif /* LEXICON_SCANNER_STATUS_H */
