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

SuccessStatus::SuccessStatus(std::string token, TokenTypes tokenType, bool endOfLine)
: LexiconScannerStatus(true),
token(token), tokenType(tokenType), endOfLine(endOfLine) {}

std::string SuccessStatus::getToken() {
    return this->token;
}

TokenTypes SuccessStatus::getTokenType() {
    return this->tokenType;
}
bool SuccessStatus::isEndOfLine() {
    return this->endOfLine;
}

// ================

FailStatus::FailStatus(std::string errorMessage)
: LexiconScannerStatus(false),
errorMessage(errorMessage) {}

std::string FailStatus::getErrorMessage() {
    return this->errorMessage;
}
