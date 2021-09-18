#include "LexiconScannerStatus.h"

//* =========================================================================
//*
//*                             STATUS RELATED
//*
//* =========================================================================

LexiconScannerStatus::LexiconScannerStatus(bool accepted)
: accepted(accepted) {}

bool LexiconScannerStatus::isAccepted() {
    return accepted;
}

// ================

SuccessStatus::SuccessStatus(std::string token, TokenTypes tokenType, TokenNames tokenName, bool endOfLine)
: LexiconScannerStatus(true) {
    this->token = new Token(token, tokenType, tokenName, endOfLine);
}

std::string SuccessStatus::getToken() {
    return this->token->getToken();
}

TokenTypes SuccessStatus::getTokenType() {
    return this->token->getType();
}

TokenNames SuccessStatus::getTokenName() {
    return this->token->getName();
}

bool SuccessStatus::isEndOfLine() {
    return this->token->isEndOfLine();
}

Token * SuccessStatus::getTokenObject() {
    return token;
}

// ================

FailStatus::FailStatus(std::string errorMessage)
: LexiconScannerStatus(false),
errorMessage(errorMessage) {}

std::string FailStatus::getErrorMessage() {
    return this->errorMessage;
}
