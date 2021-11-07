#include "ExpressionEvaluator.h"



PseudoToken::PseudoToken(USint resolvedValue) :
    Token("", TokenTypes::tNULL_TYPE, TokenNames::nNULL_TYPE, false),
    resolvedValue(resolvedValue)
{}

USint PseudoToken::getSolvedValue() {
    return resolvedValue;
}


ExpressionEvaluator::ExpressionEvaluator(Expression * expression, SymbolTable * symbolTable) :
    priority1(false),
    priority2(false),
    priority3(false),
    priority4(false),
    expression(expression),
    symbolTable(symbolTable),
    symbolCouldNotBeResolved(false)
{
    solve(0, true);
}

/*

    PASSOS
    1. procurar o parenteses mais interno
    2. encontrar o limite do parenteses mais interno
    3. resolver as operações conforme a prioridade
        3.1 buscar operação
        3.2 identificar operandos
        3.3 se o operando for símbolo, resolvê-lo pela tabela (a + b)
        3.4 se o operando for literal, traduzir para um literal processável (a + 2)
        3.5 se o operando for um valor já resolvido, reutilizá-lo
        3.6 efetuar operação
        3.7 substituir os tokens envolvidos na operação por um pseudo-token que 
        indique o valor resolvido
    4. remover parenteses que envolvem valor resolvido
    5. se ainda houver parenteses voltar ao passo 1
    6. se não houver parenteses, resolver as operações na raiz da expressão
    7. guardar resultado final


    1. 'not' '+'(unario) '-' (unário)
    2. '*' '/' 'mod' 'and'
    3. '+' '-' 'or' 'xor'
    4. 'eq' 'ne' 'lt' 'le' 'gt' 'ge'

*/
void ExpressionEvaluator::solve(int precedenceBegin, bool root) {

    int precedenceEnd = -1;

    for(int i = precedenceBegin; i < (int) expression->size(); i++) {
        if(expression->at(i)->getType() == TokenTypes::tExpPRECEDENCE_OP) {
            solve(i, false);
            if(symbolCouldNotBeResolved) return;
            continue;
        }
        if(expression->at(i)->getType() == TokenTypes::tExpPRECEDENCE_ED) {
            precedenceEnd = i;
            break;
        }
    }

    int i = precedenceBegin+1;
    if(root) {
        i = 0;
        precedenceEnd = (int) expression->size();
    }

    while(precedenceBegin+(root ? 1 : 2) < precedenceEnd) {

        if(!priority1) {
            if(
                    expression->at(i)->getName() == TokenNames::nExpADD ||
                    expression->at(i)->getName() == TokenNames::nExpSUB ||
                    expression->at(i)->getName() == TokenNames::nExpNOT
            ) {
                if(i-1 >= 0 && !isOperand(expression->at(i-1)) && isOperand(expression->at(i+1))) {
                    solveSymbol(i+1);
                    if(symbolCouldNotBeResolved) return;
                    swap(i, (Token *) solvePriority1(i, precedenceEnd));
                    delete expression->at(i+1);
                    expression->erase(expression->begin()+i+1);
                    i--;
                    precedenceEnd--;
                }
            }
        }
        else if(!priority2) {
            if(
                    expression->at(i)->getName() == TokenNames::nExpMUL ||
                    expression->at(i)->getName() == TokenNames::nExpDIV ||
                    expression->at(i)->getName() == TokenNames::nExpMOD ||
                    expression->at(i)->getName() == TokenNames::nExpAND
            ) {
                if(i-1 >= 0 && isOperand(expression->at(i-1)) && isOperand(expression->at(i+1))) {
                    solveSymbol(i-1);
                    if(symbolCouldNotBeResolved) return;
                    solveSymbol(i+1);
                    if(symbolCouldNotBeResolved) return;
                    swap(i, (Token *) solvePriority2(i, precedenceEnd));
                    delete expression->at(i+1);
                    expression->erase(expression->begin()+i+1);
                    delete expression->at(i-1);
                    expression->erase(expression->begin()+i-1);
                    i -= 2;
                    precedenceEnd -= 2;
                }
            }
        }
        else if(!priority3) {
            if(
                    expression->at(i)->getName() == TokenNames::nExpADD ||
                    expression->at(i)->getName() == TokenNames::nExpSUB ||
                    expression->at(i)->getName() == TokenNames::nExpOR ||
                    expression->at(i)->getName() == TokenNames::nExpXOR
            ) {
                if(i-1 >= 0 && isOperand(expression->at(i-1)) && isOperand(expression->at(i+1))) {
                    solveSymbol(i-1);
                    if(symbolCouldNotBeResolved) return;
                    solveSymbol(i+1);
                    if(symbolCouldNotBeResolved) return;
                    swap(i, (Token *) solvePriority3(i, precedenceEnd));
                    delete expression->at(i+1);
                    expression->erase(expression->begin()+i+1);
                    delete expression->at(i-1);
                    expression->erase(expression->begin()+i-1);
                    i -= 2;
                    precedenceEnd -= 2;
                }
            }
        }
        else if(!priority4) { // 4. 'eq' 'ne' 'lt' 'le' 'gt' 'ge'
            if(
                    expression->at(i)->getName() == TokenNames::nExpEQ ||
                    expression->at(i)->getName() == TokenNames::nExpNE ||
                    expression->at(i)->getName() == TokenNames::nExpLT ||
                    expression->at(i)->getName() == TokenNames::nExpLE ||
                    expression->at(i)->getName() == TokenNames::nExpGT ||
                    expression->at(i)->getName() == TokenNames::nExpGE
            ) {
                if(i-1 >= 0 && isOperand(expression->at(i-1)) && isOperand(expression->at(i+1))) {
                    solveSymbol(i-1);
                    if(symbolCouldNotBeResolved) return;
                    solveSymbol(i+1);
                    if(symbolCouldNotBeResolved) return;
                    swap(i, (Token *) solvePriority4(i, precedenceEnd));
                    delete expression->at(i+1);
                    expression->erase(expression->begin()+i+1);
                    delete expression->at(i-1);
                    expression->erase(expression->begin()+i-1);
                    i -= 2;
                    precedenceEnd -= 2;
                }
            }
        }

        i++;

        if(i == precedenceEnd && !priority1) {
            priority1 = true;
            i = precedenceBegin;
        } else if(i == precedenceEnd && !priority2) {
            priority2 = true;
            i = precedenceBegin;
        } else if(i == precedenceEnd && !priority3) {
            priority3 = true;
            i = precedenceBegin;
        } else if(i == precedenceEnd && !priority4) {
            priority4 = true;
        }

    }

    delete expression->at(precedenceEnd);
    expression->erase(expression->begin()+precedenceEnd);
    delete expression->at(precedenceBegin);
    expression->erase(expression->begin()+precedenceBegin);

    priority1 = priority2 = priority3 = priority4 = false;

}

bool ExpressionEvaluator::isOperand(Token * token) {
    return (
        token->getType() == TokenTypes::tIDENTIFIER ||
        token->getType() == TokenTypes::tNULL_TYPE ||
        token->getType() == TokenTypes::tDECIMAL ||
        token->getType() == TokenTypes::tHEXADECIMAL ||
        token->getType() == TokenTypes::tBINARY ||
        token->getType() == TokenTypes::tCHARACTERE
    );
}

void ExpressionEvaluator::solveSymbol(int k) {
    if(expression->at(k)->getType() == TokenTypes::tNULL_TYPE) return;
    Token * solvedSymbol = (Token *) solveSymbol(expression->at(k));
    if(symbolCouldNotBeResolved) return;
    swap(k, solvedSymbol);
}

void ExpressionEvaluator::swap(int k, Token * token) {
    expression->insert(expression->begin()+k, token);
    delete expression->at(k+1);
    expression->erase(expression->begin()+k+1);
}

PseudoToken * ExpressionEvaluator::solveSymbol(Token * token) {

    std::string v;

    switch(token->getType()) {
        case TokenTypes::tIDENTIFIER:
            if(symbolTable->find(token->getToken()) != symbolTable->end()) {
                return new PseudoToken((USint) std::stoi(
                    symbolTable->find(token->getToken())->second->value
                ));
            }
            symbolCouldNotBeResolved = true;
            return nullptr;
        case TokenTypes::tDECIMAL:
            v = token->getToken();
            char d = v[v.size() - 1];
            return new PseudoToken((USint) std::stoi(
                d == 'd' || d == 'D' ?
                    v.substr(0, v.size() - 1) :
                    v
            ));
        case TokenTypes::tHEXADECIMAL:
            v = token->getToken();
            return new PseudoToken((USint) std::stoul(v.substr(0, v.size() - 1), nullptr, 16));
        case TokenTypes::tBINARY:
            v = token->getToken();
            return new PseudoToken((USint) std::stoul(v.substr(0, v.size() - 1), nullptr, 2));
        case TokenTypes::tCHARACTERE:
            v = token->getToken();
            if(v.size() == 4) {
                switch(v[1]) {
                    case '0': new PseudoToken((USint) '\0');
                    case 'a': new PseudoToken((USint) '\a');
                    case 'b': new PseudoToken((USint) '\b');
                    case 't': new PseudoToken((USint) '\t');
                    case 'n': new PseudoToken((USint) '\n');
                    case 'v': new PseudoToken((USint) '\v');
                    case 'f': new PseudoToken((USint) '\f');
                    case 'r': new PseudoToken((USint) '\r');
                    case '\"': new PseudoToken((USint) '\"');
                    case '\'': new PseudoToken((USint) '\'');
                    case '?': new PseudoToken((USint) '\?');
                    case '\\': new PseudoToken((USint) '\\');
                    default: return nullptr;
                }
            }
            return new PseudoToken((USint) v[1]);
    }

    return nullptr;

}

PseudoToken * ExpressionEvaluator::solvePriority1(int k, int max) {

    PseudoToken * operand = (PseudoToken *) expression->at(k+1);

    switch(expression->at(k)->getName()) {
        case TokenNames::nExpADD:
            return new PseudoToken( + operand->getSolvedValue());
        case TokenNames::nExpSUB:
            return new PseudoToken( - operand->getSolvedValue());
        case TokenNames::nExpNOT:
            return new PseudoToken( ~ operand->getSolvedValue());
    }

    return nullptr;

}

PseudoToken * ExpressionEvaluator::solvePriority2(int k, int max) {

    PseudoToken * operand1 = (PseudoToken *) expression->at(k-1);
    PseudoToken * operand2 = (PseudoToken *) expression->at(k+1);

    switch(expression->at(k)->getName()) {
        case TokenNames::nExpMUL:
            return new PseudoToken( 
                operand1->getSolvedValue() * operand2->getSolvedValue());
        case TokenNames::nExpDIV:
            return new PseudoToken(
                 operand1->getSolvedValue() / operand2->getSolvedValue());
        case TokenNames::nExpMOD:
            return new PseudoToken(
                operand1->getSolvedValue() % operand2->getSolvedValue());
        case TokenNames::nExpAND:
            return new PseudoToken(
                operand1->getSolvedValue() & operand2->getSolvedValue());
    }

    return nullptr;

}

PseudoToken * ExpressionEvaluator::solvePriority3(int k, int max) {
    
    PseudoToken * operand1 = (PseudoToken *) expression->at(k-1);
    PseudoToken * operand2 = (PseudoToken *) expression->at(k+1);

    switch(expression->at(k)->getName()) {
        case TokenNames::nExpADD:
            return new PseudoToken( 
                operand1->getSolvedValue() + operand2->getSolvedValue());
        case TokenNames::nExpSUB:
            return new PseudoToken(
                 operand1->getSolvedValue() - operand2->getSolvedValue());
        case TokenNames::nExpOR:
            return new PseudoToken( 
                operand1->getSolvedValue() | operand2->getSolvedValue());
        case TokenNames::nExpXOR:
            return new PseudoToken(
                 operand1->getSolvedValue() ^ operand2->getSolvedValue());
    }

    return nullptr;

}

// 4. 'eq' 'ne' 'lt' 'le' 'gt' 'ge'
PseudoToken * ExpressionEvaluator::solvePriority4(int k, int max) {
    
    PseudoToken * operand1 = (PseudoToken *) expression->at(k-1);
    PseudoToken * operand2 = (PseudoToken *) expression->at(k+1);

    switch(expression->at(k)->getName()) {
        case TokenNames::nExpEQ:
            return new PseudoToken(
                operand1->getSolvedValue() == operand2->getSolvedValue() ? 0b1111111111111111 : 0);
        case TokenNames::nExpNE:
            return new PseudoToken(
                operand1->getSolvedValue() != operand2->getSolvedValue() ? 0b1111111111111111 : 0);
        case TokenNames::nExpLT:
            return new PseudoToken(
                operand1->getSolvedValue() < operand2->getSolvedValue() ? 0b1111111111111111 : 0);
        case TokenNames::nExpLE:
            return new PseudoToken(
                operand1->getSolvedValue() <= operand2->getSolvedValue() ? 0b1111111111111111 : 0);
        case TokenNames::nExpGT:
            return new PseudoToken(
                operand1->getSolvedValue() > operand2->getSolvedValue() ? 0b1111111111111111 : 0);
        case TokenNames::nExpGE:
            return new PseudoToken(
                operand1->getSolvedValue() >= operand2->getSolvedValue() ? 0b1111111111111111 : 0);
    }

    return nullptr;

}

USint ExpressionEvaluator::getValue() {
    return ((PseudoToken *) expression->at(0))->getSolvedValue();
}

bool ExpressionEvaluator::isSymbolCouldNotBeResolved() {
    return symbolCouldNotBeResolved;
}
