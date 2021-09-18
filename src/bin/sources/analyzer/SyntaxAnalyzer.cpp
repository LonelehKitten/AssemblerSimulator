#include "SyntaxAnalyzer.h"

SyntaxAnalyzer::SyntaxAnalyzer() {
    this->scanner = new LexiconScanner();
    this->stack = new std::stack<std::string>();
    this->macroStack = new std::stack<std::string>();
}

void SyntaxAnalyzer::set(std::string line, bool last) {
    this->line = line;
    this->last = last;
    this->state = SyntaxAutomatons::qBegin;
    this->aux1 = "";
    this->aux2 = "";
    this->aux3 = "";
    if(this->vaux != nullptr) this->vaux->clear();
    this->isMacroContent = false;
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

std::vector<Token *> * SyntaxAnalyzer::getRawRow() {
    return &row;
}

Token * SyntaxAnalyzer::getLastToken() {
    return this->row.back();
}

void SyntaxAnalyzer::acceptMacroContent() {
    row[0]->~Token();
    row.clear();
    row.emplace_back(new Token(line, TokenTypes::tMACROCONTENT, TokenNames::nMACROCONTENT, true));
    isMacroContent = true;
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

Semantic * SyntaxAnalyzer::getRow() {

    int t;
    std::vector<std::vector<Token *> *> * params = new std::vector<std::vector<Token *> *>();
    std::vector<Token *> * expression;
    Token * t1 = row[0], * t2;
    if(row.size() > 1) t2 = row[1];

    if(!macroStack->empty() && t1->getType() == TokenTypes::tMACROCONTENT) {
        return new MacroContent(line);
    }

    switch (t1->getType()) {
        case TokenTypes::tIDENTIFIER:
            switch (t2->getType()) {
                case TokenTypes::tBLOCKDEF:
                    if(t2->getName() == TokenNames::nDirSEGMENT) {
                        return new Segment(this->line, this->getAux1());
                    }
                    else if(t2->getName() == TokenNames::nDirPROC) {
                        return new Proc(this->line, this->getAux1());
                    }
                    else if(t2->getName() == TokenNames::nDirMACRO) {
                        return new Macro(this->line, this->getAux1(), this->vaux);
                    }
                    break;
                case TokenTypes::tBLOCKEND:
                    if(t2->getName() == TokenNames::nDirENDS) {
                        return new EndS(this->line, this->getAux1());
                    }
                    else if(t2->getName() == TokenNames::nDirENDP) {
                        return new EndP(this->line, this->getAux1());
                    }
                    break;
                case TokenTypes::tIDENTIFIER:
                    t = -1;
                    while(t < (int) row.size()) {
                        t++;
                        params->emplace_back(getExpression(t, t));
                    }
                    return new MacroCall(this->line, params);
                default:
                    break;
            }

        case TokenTypes::tASSUME:
            return new Assume(this->line, this->aux1, this->getAux2());

        case TokenTypes::tVARDEF:
            if(row[1]->getType() == TokenTypes::tUNDEFINED) {
                return new Dw(this->line, nullptr);
            }
            expression = getExpression(1, t);
            return new Dw(this->line, expression);

        case TokenTypes::tEND:
            return new End(this->line, this->aux1);

        case TokenTypes::tBLOCKEND:
            if(t1->getName() == TokenNames::nDirENDM)
                return new End(this->line, this->aux1);
            break;

        case TokenTypes::tOPERATION:
            switch (t1->getName()) {
                case TokenNames::nOpADD:
                    if(row[3]->getType() == TokenTypes::tREGISTER) {
                        return new Add(this->line, this->aux1);
                    }
                    expression = getExpression(3, t);
                    return new Add(this->line, expression);

                case TokenNames::nOpSUB:
                    if(row[3]->getType() == TokenTypes::tREGISTER) {
                        return new Sub(this->line, this->aux1);
                    }
                    expression = getExpression(3, t);
                    return new Sub(this->line, expression);

                case TokenNames::nOpDIV:
                    return new Mul(this->line, this->aux1);

                case TokenNames::nOpMUL:
                    return new Div(this->line, this->aux1);

                case TokenNames::nOpMOV:
                    if(t2->getType() == TokenTypes::tREGISTER) {
                        if(row[3]->getType() == TokenTypes::tREGISTER) {
                            return new Mov(this->line, this->aux1, this->aux2);
                        }
                        expression = getExpression(3, t);
                        return new Mov(this->line, this->aux1, expression, (t < (int) row.size()));
                    }
                    expression = getExpression(1, t);
                    return new Mov(this->line, expression, this->aux2,
                        row[t]->getType() == TokenTypes::tINDEX_OP
                    );

                    /*
                case TokenNames::nOpOR:
                    return new Or(this->line, this->getAux1());
                    break;
                case TokenNames::nOpAND:
                    return new And(this->line, this->getAux1());
                    break;
                case TokenNames::nOpXOR:
                    return new Xor(this->line, this->getAux1());
                    break;
                    */
                case TokenNames::nOpPUSH:
                    return new Push(this->line);
                case TokenNames::nOpPOP:
                    return new Pop(this->line);
                case TokenNames::nOpPUSHF:
                    return new Pushf(this->line);
                case TokenNames::nOpPOPF:
                    return new Popf(this->line);
                default:
                    break;
            }
            break;
        default:
            break;
    }

    return new Add("ADD AX, DX", "DX");

}

std::vector<Token *> * SyntaxAnalyzer::getExpression(int it, int& pointer) {

    std::vector<Token *> * expression = new std::vector<Token *>();

    while(
          it < (int) row.size() &&
          (row[it]->getType() != TokenTypes::tSEPARATOR && row[it]->getType() != TokenTypes::tINDEX_OP)
    ) {
        expression->emplace_back(row[it]);
        it++;
    }

    pointer = it;

    return expression;

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

std::function<bool(SyntaxAnalyzer *)> SyntaxAnalyzer::getEndpoint() {
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
            (transition->getPop() == "" || ( !this->stack->empty() && this->stack->top() == transition->getPop())) &&
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

std::stack<std::string> *SyntaxAnalyzer::getMacroStack() const
{
    return macroStack;
}

void SyntaxAnalyzer::undoScan() {
    this->error = false;
    this->scanner->undo();
}
