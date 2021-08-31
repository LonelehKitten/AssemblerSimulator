#include "LexiconScannerStatus.hpp"

LexiconScannerStatus::LexiconScannerStatus(bool accepted) {
    this->accepted = accepted;
}

bool LexiconScannerStatus::isAccepted() {
    return accepted;
}

// ================

SuccessStatus::SuccessStatus(std::string token, int tokenType, bool endOfLine)
: LexiconScannerStatus::LexiconScannerStatus(true) {
    this->token = token;
    this->tokenType = tokenType;
    this->endOfLine = endOfLine;
}

std::string SuccessStatus::getToken() {
    return this->token;
}

int SuccessStatus::getTokenType() {
    return this->tokenType;
}
bool SuccessStatus::isEndOfLine() {
    return this->endOfLine;
}

// ================

FailStatus::FailStatus(std::string errorMessage) 
: LexiconScannerStatus::LexiconScannerStatus(false) {
    this->errorMessage = errorMessage;
}

std::string FailStatus::getErrorMessage() {
    return this->errorMessage;
}