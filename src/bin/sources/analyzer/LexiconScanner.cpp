#include "LexiconScanner.h"


//* =========================================================================
//*
//*                        LEXICON SCANNER RELATED
//*
//* =========================================================================

// constructor
LexiconScanner::LexiconScanner() {

    this->stack = new std::stack<std::string>();

    initTokenSet();

    // Aqui vai o dicionário de estados iniciais pelo tokenType, e.g (type: state)
    this->initialStates[TokenTypes::tBLOCKDEF] = Automatons::qBegin_labelPattern;
    this->initialStates[TokenTypes::tBLOCKEND] = Automatons::qBegin_labelPattern;
    this->initialStates[TokenTypes::tEND] = Automatons::qBegin_labelPattern;
    // ...

    LexiconScanner::createDictionary(this->tokenSet, this->tokens);
}

void LexiconScanner::initTokenSet() {

    this->tokenSet[TokenNames::nDirMACRO]           = new LexiconScanner::TokenSetUnit("macro",     TokenTypes::tBLOCKDEF);
    this->tokenSet[TokenNames::nDirPROC]            = new LexiconScanner::TokenSetUnit("proc",      TokenTypes::tBLOCKDEF);
    this->tokenSet[TokenNames::nDirSEGMENT]         = new LexiconScanner::TokenSetUnit("segment",   TokenTypes::tBLOCKDEF);
    this->tokenSet[TokenNames::nDirENDM]            = new LexiconScanner::TokenSetUnit("endm",      TokenTypes::tBLOCKEND);
    this->tokenSet[TokenNames::nDirENDP]            = new LexiconScanner::TokenSetUnit("endp",      TokenTypes::tBLOCKEND);
    this->tokenSet[TokenNames::nDirENDS]            = new LexiconScanner::TokenSetUnit("ends",      TokenTypes::tBLOCKEND);
    this->tokenSet[TokenNames::nDirEND]             = new LexiconScanner::TokenSetUnit("end",       TokenTypes::tEND);
    // OPERAÇÕES
    this->tokenSet[TokenNames::nOpADD]              = new LexiconScanner::TokenSetUnit("add",       TokenTypes::tOPERATION);
    this->tokenSet[TokenNames::nOpSUB]              = new LexiconScanner::TokenSetUnit("sub",       TokenTypes::tOPERATION);
    this->tokenSet[TokenNames::nOpMUL]              = new LexiconScanner::TokenSetUnit("mul",       TokenTypes::tOPERATION);
    this->tokenSet[TokenNames::nOpDIV]              = new LexiconScanner::TokenSetUnit("div",       TokenTypes::tOPERATION);
    this->tokenSet[TokenNames::nOpOR]               = new LexiconScanner::TokenSetUnit("or",        TokenTypes::tOPERATION);
    this->tokenSet[TokenNames::nOpAND]              = new LexiconScanner::TokenSetUnit("and",       TokenTypes::tOPERATION);
    this->tokenSet[TokenNames::nOpXOR]              = new LexiconScanner::TokenSetUnit("xor",       TokenTypes::tOPERATION);
    this->tokenSet[TokenNames::nOpNOT]              = new LexiconScanner::TokenSetUnit("not",       TokenTypes::tOPERATION);
    this->tokenSet[TokenNames::nOpJMP]              = new LexiconScanner::TokenSetUnit("jmp",       TokenTypes::tOPERATION);
    this->tokenSet[TokenNames::nOpJE]               = new LexiconScanner::TokenSetUnit("je",        TokenTypes::tOPERATION);
    this->tokenSet[TokenNames::nOpJNZ]              = new LexiconScanner::TokenSetUnit("jnz",       TokenTypes::tOPERATION);
    this->tokenSet[TokenNames::nOpJZ]               = new LexiconScanner::TokenSetUnit("jz",        TokenTypes::tOPERATION);
    this->tokenSet[TokenNames::nOpJP]               = new LexiconScanner::TokenSetUnit("jp",        TokenTypes::tOPERATION);
    this->tokenSet[TokenNames::nOpCALL]             = new LexiconScanner::TokenSetUnit("call",      TokenTypes::tOPERATION);
    this->tokenSet[TokenNames::nOpINT]              = new LexiconScanner::TokenSetUnit("int",       TokenTypes::tOPERATION);
    this->tokenSet[TokenNames::nOpRET]              = new LexiconScanner::TokenSetUnit("ret",       TokenTypes::tOPERATION);
    this->tokenSet[TokenNames::nOpMOV]              = new LexiconScanner::TokenSetUnit("mov",       TokenTypes::tOPERATION);
    this->tokenSet[TokenNames::nOpPOP]              = new LexiconScanner::TokenSetUnit("pop",       TokenTypes::tOPERATION);
    this->tokenSet[TokenNames::nOpPUSH]             = new LexiconScanner::TokenSetUnit("push",      TokenTypes::tOPERATION);
    this->tokenSet[TokenNames::nOpPOPF]             = new LexiconScanner::TokenSetUnit("popf",      TokenTypes::tOPERATION);
    this->tokenSet[TokenNames::nOpPUSHF]            = new LexiconScanner::TokenSetUnit("pushf",     TokenTypes::tOPERATION);
    // REGISTRADORES
    this->tokenSet[TokenNames::nRegAX]              = new LexiconScanner::TokenSetUnit("ax",        TokenTypes::tREGISTER);
    this->tokenSet[TokenNames::nRegDX]              = new LexiconScanner::TokenSetUnit("dx",        TokenTypes::tREGISTER);
    this->tokenSet[TokenNames::nRegSI]              = new LexiconScanner::TokenSetUnit("si",        TokenTypes::tREGISTER);
    this->tokenSet[TokenNames::nRegSP]              = new LexiconScanner::TokenSetUnit("sp",        TokenTypes::tREGISTER);
    this->tokenSet[TokenNames::nRegCS]              = new LexiconScanner::TokenSetUnit("cs",        TokenTypes::tREGISTER);
    this->tokenSet[TokenNames::nRegDS]              = new LexiconScanner::TokenSetUnit("ds",        TokenTypes::tREGISTER);
    this->tokenSet[TokenNames::nRegSS]              = new LexiconScanner::TokenSetUnit("ss",        TokenTypes::tREGISTER);
    // OPERAÇÕES DE EXPRESSÃO (MONTADOR)
    this->tokenSet[TokenNames::nExpADD]             = new LexiconScanner::TokenSetUnit("+",         TokenTypes::tExpARITHMETICu);
    this->tokenSet[TokenNames::nExpSUB]             = new LexiconScanner::TokenSetUnit("-",         TokenTypes::tExpARITHMETICu);
    this->tokenSet[TokenNames::nExpMUL]             = new LexiconScanner::TokenSetUnit("*",         TokenTypes::tExpARITHMETICb);
    this->tokenSet[TokenNames::nExpDIV]             = new LexiconScanner::TokenSetUnit("/",         TokenTypes::tExpARITHMETICb);
    this->tokenSet[TokenNames::nExpMOD]             = new LexiconScanner::TokenSetUnit("mod",       TokenTypes::tExpARITHMETICb);
    this->tokenSet[TokenNames::nExpOR]              = new LexiconScanner::TokenSetUnit("or",        TokenTypes::tExpLOGICALb);
    this->tokenSet[TokenNames::nExpAND]             = new LexiconScanner::TokenSetUnit("and",       TokenTypes::tExpLOGICALb);
    this->tokenSet[TokenNames::nExpXOR]             = new LexiconScanner::TokenSetUnit("xor",       TokenTypes::tExpLOGICALb);
    this->tokenSet[TokenNames::nExpNOT]             = new LexiconScanner::TokenSetUnit("not",       TokenTypes::tExpLOGICALu);
    this->tokenSet[TokenNames::nExpEQ]              = new LexiconScanner::TokenSetUnit("eq",        TokenTypes::tExpRELATIONALb);
    this->tokenSet[TokenNames::nExpNE]              = new LexiconScanner::TokenSetUnit("ne",        TokenTypes::tExpRELATIONALb);
    this->tokenSet[TokenNames::nExpLT]              = new LexiconScanner::TokenSetUnit("lt",        TokenTypes::tExpRELATIONALb);
    this->tokenSet[TokenNames::nExpLE]              = new LexiconScanner::TokenSetUnit("le",        TokenTypes::tExpRELATIONALb);
    this->tokenSet[TokenNames::nExpGT]              = new LexiconScanner::TokenSetUnit("gt",        TokenTypes::tExpRELATIONALb);
    this->tokenSet[TokenNames::nExpGE]              = new LexiconScanner::TokenSetUnit("ge",        TokenTypes::tExpRELATIONALb);
    this->tokenSet[TokenNames::nExpPRECEDENCE_OP]   = new LexiconScanner::TokenSetUnit("(",         TokenTypes::tExpPRECEDENCE_OP);
    this->tokenSet[TokenNames::nExpPRECEDENCE_ED]   = new LexiconScanner::TokenSetUnit(")",         TokenTypes::tExpPRECEDENCE_ED);
    // DECLARATION
    this->tokenSet[TokenNames::nASSUME]             = new LexiconScanner::TokenSetUnit("assume",    TokenTypes::tASSUME);
    this->tokenSet[TokenNames::nDW]                 = new LexiconScanner::TokenSetUnit("dw",        TokenTypes::tVARDEF);
    this->tokenSet[TokenNames::nDUP]                = new LexiconScanner::TokenSetUnit("dup",       TokenTypes::tVECFILL);
    this->tokenSet[TokenNames::nEQU]                = new LexiconScanner::TokenSetUnit("equ",       TokenTypes::tCONSTDEF);
    this->tokenSet[TokenNames::nUNDEFINED]          = new LexiconScanner::TokenSetUnit("?",         TokenTypes::tUNDEFINED);
    // OTHERS
    this->tokenSet[TokenNames::nINDEX_OP]           = new LexiconScanner::TokenSetUnit("[",         TokenTypes::tINDEX_OP);
    this->tokenSet[TokenNames::nINDEX_ED]           = new LexiconScanner::TokenSetUnit("]",         TokenTypes::tINDEX_ED);
    this->tokenSet[TokenNames::nSEPARATOR]          = new LexiconScanner::TokenSetUnit(",",         TokenTypes::tSEPARATOR);
    this->tokenSet[TokenNames::nCOLON]              = new LexiconScanner::TokenSetUnit(":",         TokenTypes::tCOLON);
    this->tokenSet[TokenNames::nPC]                 = new LexiconScanner::TokenSetUnit("$",         TokenTypes::tPC);
    this->tokenSet[TokenNames::nORG]                = new LexiconScanner::TokenSetUnit("org",       TokenTypes::tORG);

    this->tokenSet[TokenNames::nOFFSET]             = new LexiconScanner::TokenSetUnit("$",         TokenTypes::tOFFSET);
    this->tokenSet[TokenNames::nSTACK]              = new LexiconScanner::TokenSetUnit("stack",       TokenTypes::tSTACK);

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
        std::unordered_map<std::string, TokenTypes>::const_iterator pair = this->tokens.find(this->token);
        tokenType = pair->second;
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

    if(!transitionEnd->isCaseSensitive()) {
        std::for_each(this->token.begin(), this->token.end(), [](char& c) {
            c = ::tolower(c);
        });
    }

    bool condition = false;

    std::unordered_map<std::string, TokenTypes>::const_iterator pair = this->tokens.find(this->token);
    if(pair != this->tokens.end()) {
        for(int i = 0; i < transitionEnd->getConditionLength(); i++) {
            if(pair->second == transitionEnd->getConditions()[i]) {
                condition = true;
                break;
            }
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
