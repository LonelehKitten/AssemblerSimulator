#include "SyntaxAnalyzer.h"

SyntaxAnalyzer::SyntaxAnalyzer() {
    this->scanner = new LexiconScanner();
    this->stack = new std::stack<std::string>();
}

void SyntaxAnalyzer::set(std::string line, bool last) {
    this->line = line;
    this->last = last;
    this->state = SyntaxAutomatons::qBegin;
}

bool SyntaxAnalyzer::init() {
    log("init");
    this->row.clear();
    this->error = false;
    this->scanner->setLine(this->line);
    while (true) {
        if(this->state(this) && !this->error) {
            return true;
        }
        if(this->error) break;
    }
    return false;
}

void SyntaxAnalyzer::trim() {
    line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    line.erase(std::find_if(line.rbegin(), line.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), line.end());
    line += '\n';
}

bool SyntaxAnalyzer::check() {

    trim();
    bool valid = false;
    for(int i = 0; i < (int) line.length(); i++) {
        if(line[i] == ';') {
            line = line.substr(0, i-1);
            break;
        }
        if(line[i] != ' ' && line[i] != '\n') {
            valid = true;
        }
    }

    return valid;
}

void SyntaxAnalyzer::getRow() {

}

LexiconScanner *SyntaxAnalyzer::getScanner() const {
    return scanner;
}

void SyntaxAnalyzer::setError(bool error) {
    this->error = error;
}

LexiconScannerStatus * SyntaxAnalyzer::getStatus() const {
    return status;
}

void SyntaxAnalyzer::log(std::string msg){
    std::cout << "------------------------------" << std::endl;
    std::cout << "%c DEBUG: " << msg << std::endl;
    std::cout << "------------------------------" << std::endl;
}

bool SyntaxAnalyzer::q(SyntaxAutomatons::Transition * transition) {

    this->error = false;
    if(transition->getLoad()) this->status = scanner->nextToken(transition->getAutomatonPattern());

    if(this->status->isAccepted()) {
        std::string token = ((SuccessStatus *) this->status)->getToken();
        std::unordered_map<std::string, TokenTypes>::const_iterator t = scanner->getTokens().find(token);
        if(
            (transition->isId() || t->second == transition->getTokenType()) &&
            (!transition->isId() || t == scanner->getTokens().end()) &&
            (transition->getPop() == "" || this->stack->top() == transition->getPop()) &&
            (transition->getToken() == "" || token == transition->getToken()) &&
            transition->getCustomFlag()
        ) {

            log(token);

            if(transition->getPop() != "") this->stack->pop();
            if(transition->getPush() != "") this->stack->push(transition->getPush());
            if(transition->getCallback() != nullptr) transition->getCallback()(this);
            if(transition->getState() != nullptr) this->state = transition->getState();
            row.emplace_back(((SuccessStatus *) this->status)->getTokenObject());

            return false;

        }
    }

    this->error = true;
    return true;

}
