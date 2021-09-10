#include "LexiconScanner.h"


//* =========================================================================
//*
//*                        LEXICON SCANNER RELATED
//*
//* =========================================================================

// constructor
LexiconScanner::LexiconScanner() {

    this->stack = new std::stack<std::string>();

    // Aqui vai o dicionário de tokens q dá acesso pelo id, e.g (id: (token, type))
    //LexiconScanner::TokenSet::ADD = new LexiconScanner::TokenSetUnit("add", LexiconScanner::TokenTypes::OPERATOR);
    this->tokenSet[TokenNames::nADD] = new LexiconScanner::TokenSetUnit("add", TokenTypes::tOPERATOR);
    // ...

    // Aqui vai o dicionário de estados iniciais pelo tokenType, e.g (type: state)
    this->initialStates[TokenTypes::tOPERATOR] = Automatons::qBegin_Operator;
    // ...

    LexiconScanner::createDictionary(this->tokenSet, this->tokens);
}

LexiconScanner::TokenSetUnit::TokenSetUnit(std::string token, TokenTypes tokenType) {
    this->token = token;
    this->tokenType = tokenType;
}

std::string LexiconScanner::TokenSetUnit::getToken() {
    return this->token;
}

TokenTypes LexiconScanner::TokenSetUnit::getTokenType() {
    return this->tokenType;
}

/* 

static createDictionary = (tokenSet) => {
        let dictionary = {};
        Object.keys(tokenSet).forEach(key => {
            dictionary[tokenSet[key][0]] = tokenSet[key][1]
        })

        console.log(dictionary)
        return dictionary;
    }

*/

void LexiconScanner::createDictionary(std::unordered_map<TokenNames, LexiconScanner::TokenSetUnit *>& tokenSet,
    std::unordered_map<std::string, TokenTypes>& tokens) {
    for(std::pair<TokenNames, LexiconScanner::TokenSetUnit *> p : tokenSet) {
        tokens[p.second->getToken()] = p.second->getTokenType();
    }
}

void LexiconScanner::setLine(std::string line) {
    this->currentChar = line[0];
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

LexiconScannerStatus * LexiconScanner::nextToken(TokenTypes tokenType) {

    start(tokenType);

    while(true) {

        log();

        if(this->state(this) && !this->error) {
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
    for(int i = 0; i < this->lineIndex; i++)
        column += ' ';
    column += '^';
    std::cout << this->line << std::endl;
    std::cout << column << std::endl;
}

void LexiconScanner::start(TokenTypes tokenType) {
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
    this->lineIndex = this->lastTokenEndPosition;
    this->currentChar = this->line[this->lineIndex];
}

void LexiconScanner::nextChar() {
    this->lineIndex++;
    this->currentChar = this->line[this->lineIndex];
}

void LexiconScanner::checkEndOfLine(bool deterministic) {
    if(deterministic) this->endOfLine = (this->line[this->lineIndex-1] == '\n');
    else this->endOfLine = (this->line[this->lineIndex] == '\n');
}

void LexiconScanner::accept(bool deterministic) {
    this->lastTokenEndPosition = this->lineIndex;
    if(!deterministic) {
        this->lastTokenEndPosition--;
    }
}

void LexiconScanner::setSuccessMessage(TokenTypes tokenType) {
    if(tokenType == TokenTypes::tNULL_TYPE) {
        tokenType = this->tokens[this->token];
    }
    this->tokenData = new SuccessStatus(this->token, tokenType, this->endOfLine);
}


/*
q = (predicates, defaultPredicate=null) => {

        for (const p in predicates) {
            if (
                predicates[p].condition &&
                (!predicates[p].pop || this.isStackValue(predicates[p].pop)) &&
                (!predicates[p].emptyStack || this.stack.isEmpty())
            ) {
                if(predicates[p].push) this.stack.push(predicates[p].push)
                if(predicates[p].pop) this.stack.pop()
                this.token += this.currentChar;
                this.state = predicates[p].state;
                return false;
            }
        }

        if(defaultPredicate) {
            if(defaultPredicate.move) this.token += this.currentChar;
            this.state = defaultPredicate.state;
        }
        else this.error = true;

        return false;

    }
*/


bool LexiconScanner::q(Automatons::Transition ** transitions, int transitionsLength, Automatons::Transition * defaultAction) {

    if(transitions != nullptr) {
                
        for(int i = 0; i < transitionsLength; i++) {
            if(
                transitions[i]->getCondition() &&
                (!transitions[i]->whatPop() || isStackValue(transitions[i]->whatPop()->c_str())) &&
                (!transitions[i]->shouldStackBeEmpty() || this->stack->empty())
            ) {
                if(transitions[i]->whatPop()) this->stack->pop();
                if(transitions[i]->whatPush()) this->stack->push(transitions[i]->whatPush()->c_str());
                this->token += this->currentChar;
                this->state = transitions[i]->getState();
                return false;
            }
        }
        
    }
    
    if(defaultAction != nullptr)
        this->state = defaultAction->getState();
    else this->error = true;

    return false;

}
/*
    qEnd = ({conditions, defaultAction=null, deterministic=true, tokenType=null, stateName='[it wasn\'t given]'}) => {
        
        this.checkEndOfLine(deterministic);
        
        //let condition = conditions;
        let condition = (this.tokens[this.token] === conditions)
        
        if(conditions instanceof Array) {
            condition = false;
            for (const c in conditions) {
                if(this.tokens[this.token] === conditions[c]) {
                    condition = true;
                    break;
                }
            }
        }

        if (condition) {
            console.log("qEnd Success")
            this.accept(deterministic)
            if(tokenType) this.setSuccessMessage(tokenType);
            else this.setSuccessMessage();
            return true
        }

        if(defaultAction) {
            this.accept(defaultAction.deterministic)
            if(defaultAction.tokenType) this.setSuccessMessage(defaultAction.tokenType);
            else this.setSuccessMessage();
            return true
        }

        this.error = true
        this.setErrorMsg(stateName);
        
        return true
    }

*/

bool LexiconScanner::qEnd(Automatons::TransitionEnd * transitionEnd) {
    checkEndOfLine(transitionEnd->isDeterministic());

    bool condition = false;

    for(int i = 0; i < transitionEnd->getConditionLength(); i++) {
        if(this->tokens[this->token] == transitionEnd->getConditions()[i]) {
            condition = true;
            break;
        }
    }

    if(condition) {
        std::cout << "qEnd Success" << std::endl;
        accept(transitionEnd->isDeterministic());
        setSuccessMessage(transitionEnd->getTokenType());
        return true;
    }

    if(transitionEnd->getDefaultAction()) {
        accept(transitionEnd->getDefaultAction()->isDeterministic());
        setSuccessMessage(transitionEnd->getDefaultAction()->getTokenType());
        return true;
    }

    this->error = true;
    return true;
}

// =========== comparadores ===========

bool LexiconScanner::isStackValue(std::string value) {
    return this->stack->top() == value;
}

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

//* =========================================================================
//*
//*                          AUTOMATONS RELATED
//*
//* =========================================================================
