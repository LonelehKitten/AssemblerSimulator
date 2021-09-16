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
        if(this->error) {
            std::cout << "syntax error" << std::endl;
            break;
        }
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

void SyntaxAnalyzer::setAux1(const std::string aux1)
{
    this->aux1 = aux1;
}

void SyntaxAnalyzer::setAux2(const std::string aux2)
{
    this->aux2 = aux2;
}

void SyntaxAnalyzer::setAux3(const std::string aux3)
{
    this->aux3 = aux3;
}

void SyntaxAnalyzer::setVAux(std::vector<std::string> * vaux)
{
    this->vaux = vaux;
}

void SyntaxAnalyzer::setMacroScope(bool macroScope)
{
    this->macroScope = macroScope;
}

const std::string SyntaxAnalyzer::getAux1() const
{
    return aux1;
}

const std::string SyntaxAnalyzer::getAux2() const
{
    return aux2;
}

const std::string SyntaxAnalyzer::getAux3() const
{
    return aux3;
}

std::vector<std::string> * SyntaxAnalyzer::getVAux() const
{
    return vaux;
}

bool SyntaxAnalyzer::isMacroScope() const
{
    return macroScope;
}

bool SyntaxAnalyzer::check() {

    trim();
    bool valid = false;
    for(int i = 0; i < (int) line.length(); i++) {
        if(line[i] == ';') {
            line = line.substr(0, i);
            line += '\n';
        }
        if(line[i] != ' ' && line[i] != '\n') {
            valid = true;
        }
        if(valid && line[i] == '\n') {
            int j;
            for(j = i-1; line[j] == ' '; j--);
            line = line.substr(0, j+1);
            line += '\n';
            break;
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

void SyntaxAnalyzer::setState(const std::function<bool(SyntaxAnalyzer *)> state)
{
    this->state = state;
}

std::function<bool(SyntaxAnalyzer *)> & SyntaxAnalyzer::getEndpoint() {
    return endpoint;
}

void SyntaxAnalyzer::setEndpoint(std::function<bool(SyntaxAnalyzer *)> endpoint) {
    this->endpoint = endpoint;
}

std::stack<std::string> *SyntaxAnalyzer::getStack() const
{
    return stack;
}

void SyntaxAnalyzer::log(std::string msg){
    std::cout << "------------------------------" << std::endl;
    std::cout << "%c DEBUG: " << msg << std::endl;
    std::cout << "------------------------------" << std::endl;
}

bool SyntaxAnalyzer::q(SyntaxAutomatons::Transition * transition) {

    this->error = false;
    if(transition->mustUndo()) this->undoScan();
    if(transition->getLoad()) this->status = scanner->nextToken(transition->getAutomatonPattern());

    if(this->status->isAccepted() && this->validate(transition)){
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

bool SyntaxAnalyzer::validate(SyntaxAutomatons::Transition * transition) {
    TokenTypes type = ((SuccessStatus *) this->status)->getTokenType();
    return transition->isId() || (
                type != TokenTypes::tIDENTIFIER &&
                type != TokenTypes::tDECIMAL &&
                type != TokenTypes::tHEXADECIMAL &&
                type != TokenTypes::tBINARY &&
                type != TokenTypes::tCHARACTERE
    );
}

void SyntaxAnalyzer::undoScan() {
    this->error = false;
    this->scanner->undo();
}
