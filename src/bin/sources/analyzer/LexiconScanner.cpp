#include "LexiconScanner.hpp"

/*

    TODO: traduzir a função transição q cada estado chama

*/

// constructor
LexiconScanner::LexiconScanner() {
    // Aqui vai o dicionário de tokens q dá acesso pelo id, e.g (id: (token, type))
    LexiconScanner::TokenSet::ADD = new LexiconScanner::TokenSetUnit("add", LexiconScanner::TokenType::OPERATOR);
    // ...

    // Aqui vai o dicionário de estados iniciais pelo tokenType, e.g (type: state)
    this->initialStates[LexiconScanner::TokenType::OPERATOR] = this->automatons.qBegin_Operator;
    // ...
}

LexiconScanner::TokenSetUnit::TokenSetUnit(std::string token, LexiconScanner::TokenType tokenType) {
    this->token = token;
    this->tokenType = tokenType;
}

std::string LexiconScanner::TokenSetUnit::getToken() {
    return this->token;
}

LexiconScanner::TokenType LexiconScanner::TokenSetUnit::getTokenType() {
    return this->tokenType;
}

void LexiconScanner::setLine(std::string line) {
    this->currentChar = line[0]
    this->line = line;
    this->endOfLine = false;
    this->lineIndex = 0;
    this->lastTokenBeginPosition = 0;
    this->lastTokenEndPosition = 0;
}

void LexiconScanner::undo() {
    this->endOfLine = false;
    this->lastTokenEndPosition = this->lastTokenBeginPosition;
}

LexiconScannerStatus * LexiconScanner::nextToken(TokenType tokenType) {

    start();

    while(true) {

        log();

        if(this->state() && !this->error) {
            return this->tokenData;
        }

        if(this->error) {
            std::cout << "ERROR" << std::endl;
            return new FailStatus("Unknown Token: " + this->token);
        }

        nextChar();

    }

}

void LexiconScanner::log() {
    std::string column = "";
    for(let i = 0; i < this->lineIndex; i++)
        column += " ";
    column += "^";
    std::cout << this->line << std::endl;
    std::cout << column << std::endl;
}

void LexiconScanner::start(int tokenType) {
    this->token = "";
    snap();
    this->lastTokenBeginPosition = this->lineIndex;
    this->state = this->initialStates[tokenType];
}

void LexiconScanner::snap() {
    this->currentChar = this->line[this->lastTokenEndPosition];
    if(isNull()) {
        while(this->line[this->lastTokenEndPosition] == ' ') {
            this->lastTokenEndPosition++;
        }
    }
    this->lineIndex = this->lineTokenEndPosition;
    this->currentChar = this->line[this->lastIndex];
}

void LexiconScanner::nextChar() {
    this->lineIndex++;
    this->currentChar = this->line[this->lineIndex];
}



// =========== comparadores ===========

//bool isUnknownToken = () => {
//    this.tokens[this.token] === undefined
//}

bool LexiconScanner::is(char character) {
    return this->currentChar == character;
}

// conjunto α U Α (alfa minúsculo união alfa maiúsculo)
bool LexiconScanner::isAlpha() {
    return (
        (this->currentChar >= 'a' && this->currentChar <= 'z') || 
        (this->currentChar >= 'A' && this->currentChar <= 'Z')
    );
}

bool LexiconScanner::isAlphaUpper() {
    return this->currentChar >= 'A' && this->currentChar <= 'Z';
}

bool LexiconScanner::isAlphaLower() {
    return this->currentChar >= 'a' && this->currentChar <= 'z';
}

// conjunto Γ (gama)
bool LexiconScanner::isNumeric() { 
    return this->currentChar >= '0' && this->currentChar <= '9';
}

// conjunto Γ (gama) ou α U Α (alfa minúsculo união alfa maiúsculo)
bool LexiconScanner::isAlphaNumeric() { 
    return isAlpha() || isNumeric();
}

// valor ε (epsilon)
bool LexiconScanner::isNull() {
    return this->currentChar == ' ' || this->currentChar == '\0';
}

// valor Φ (fi)
bool LexiconScanner::isLineBreak() {
    return this->currentChar == '\n';
}

// conjunto Σ (sigma)
bool LexiconScanner::isSpecial() {
    return !isAlpha() && !isNumeric() && !isLineBreak() && !isNull();
}