#include "ExpressionEvaluator.h"
#include <iostream>

#ifdef PRODUCTION_MODE
#include "../InterfaceBus.h"
#endif


PseudoToken::PseudoToken(USint resolvedValue, bool is_const) :
    Token("", TokenTypes::tNULL_TYPE, TokenNames::nNULL_TYPE, false),
    resolvedValue(resolvedValue),
    is_const(is_const)
{}

USint PseudoToken::getSolvedValue() {
    return resolvedValue;
}

bool PseudoToken::isConst() {
    return is_const;
}


ExpressionEvaluator::ExpressionEvaluator(Expression * expression,
    SegmentDef * programSegment, SegmentDef * dataSegment, 
    SymbolTable * segmentTable, SymbolTable * globalSymbols) :
    priority1(false),
    priority2(false),
    priority3(false),
    priority4(false),
    expression(new Expression(*expression)),
    originalExpression(expression),
    programSegment(programSegment),
    dataSegment(dataSegment),
    segmentTable(segmentTable),
    globalSymbols(globalSymbols),
    symbolCouldNotBeResolved(false)
{
    solve(-1, true);
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

    if(expression->size() == 1) {
        expression->emplace_back(solveSymbol(expression->at(0)));
        expression->erase(expression->begin());
        return;
    }
    // 0 1 2 3 4 5 6
    // 2 * 5
    int precedenceEnd = -1;
    for(int i = precedenceBegin+1; i < (int) expression->size(); i++) {

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

    do {

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
                    expression->erase(expression->begin()+i+1);
                    i = root ? -1 : precedenceBegin;
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
                if(i > 0 && isOperand(expression->at(i-1)) && isOperand(expression->at(i+1))) {
                    solveSymbol(i-1);
                    if(symbolCouldNotBeResolved) return;
                    solveSymbol(i+1);
                    if(symbolCouldNotBeResolved) return;
                    swap(i, (Token *) solvePriority2(i, precedenceEnd));
                    expression->erase(expression->begin()+i+1);
                    expression->erase(expression->begin()+i-1);
                    i = root ? -1 : precedenceBegin;
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
                if(i > 0 && isOperand(expression->at(i-1)) && isOperand(expression->at(i+1))) {
                    solveSymbol(i-1);
                    if(symbolCouldNotBeResolved) return;
                    solveSymbol(i+1);
                    if(symbolCouldNotBeResolved) return;
                    swap(i, (Token *) solvePriority3(i, precedenceEnd));
                    expression->erase(expression->begin()+i+1);
                    expression->erase(expression->begin()+i-1);
                    i = root ? -1 : precedenceBegin;
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
                if(i > 0 && isOperand(expression->at(i-1)) && isOperand(expression->at(i+1))) {
                    solveSymbol(i-1);
                    if(symbolCouldNotBeResolved) return;
                    solveSymbol(i+1);
                    if(symbolCouldNotBeResolved) return;
                    swap(i, (Token *) solvePriority4(i, precedenceEnd));
                    expression->erase(expression->begin()+i+1);
                    expression->erase(expression->begin()+i-1);
                    i = root ? -1 : precedenceBegin;
                    precedenceEnd -= 2;
                }
            }
        }

        i++;

        if(i == precedenceEnd && !priority1) {
            priority1 = true;
            i = precedenceBegin+1;
        } else if(i == precedenceEnd && !priority2) {
            priority2 = true;
            i = precedenceBegin+1;
        } else if(i == precedenceEnd && !priority3) {
            priority3 = true;
            i = precedenceBegin+1;
        } else if(i == precedenceEnd && !priority4) {
            priority4 = true;
        }

    } while(precedenceBegin+2 < precedenceEnd);

    if(!root) {
        expression->erase(expression->begin()+precedenceEnd);
        expression->erase(expression->begin()+precedenceBegin);
    }

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
            if(globalSymbols->find(token->getToken()) != globalSymbols->end() &&
                globalSymbols->find(token->getToken())->second->value != "??") {
                return new PseudoToken((USint) std::stoi(
                    globalSymbols->find(token->getToken())->second->value
                ), !globalSymbols->find(token->getToken())->second->isVar);
            }
            else if(segmentTable->find(token->getToken()) != segmentTable->end()) {
                return new PseudoToken((USint) std::stoi(
                    segmentTable->find(token->getToken())->second->value
                ), true);
            }
            else if(programSegment->getSymbol(token->getToken()) != nullptr &&
                programSegment->getSymbol(token->getToken())->value != "??") {
                
                return new PseudoToken((USint) std::stoi(
                    programSegment->getSymbol(token->getToken())->value
                ), !programSegment->getSymbol(token->getToken())->isVar);

            }
            else if(dataSegment->getSymbol(token->getToken()) != nullptr &&
                dataSegment->getSymbol(token->getToken())->value != "??") {
                
                return new PseudoToken((USint) std::stoi(
                    dataSegment->getSymbol(token->getToken())->value
                ), !dataSegment->getSymbol(token->getToken())->isVar);
            }
            symbolCouldNotBeResolved = true;
            return nullptr;
        case TokenTypes::tDECIMAL:
        {
            LOG("decimal")
            v = token->getToken();
            char d = v[v.size() - 1];
            return new PseudoToken((USint) std::stoi(
                d == 'd' || d == 'D' ?
                    v.substr(0, v.size() - 1) :
                    v
            ), true);
        }
        case TokenTypes::tHEXADECIMAL:
            v = token->getToken();
            return new PseudoToken((USint) std::stoul(v.substr(0, v.size() - 1), nullptr, 16), true);
        case TokenTypes::tBINARY:
            v = token->getToken();
            return new PseudoToken((USint) std::stoul(v.substr(0, v.size() - 1), nullptr, 2), true);
        case TokenTypes::tCHARACTERE:
            v = token->getToken();
            if(v.size() == 4) {
                switch(v[1]) {
                    case '0': return new PseudoToken((USint) '\0', true);
                    case 'a': return new PseudoToken((USint) '\a', true);
                    case 'b': return new PseudoToken((USint) '\b', true);
                    case 't': return new PseudoToken((USint) '\t', true);
                    case 'n': return new PseudoToken((USint) '\n', true);
                    case 'v': return new PseudoToken((USint) '\v', true);
                    case 'f': return new PseudoToken((USint) '\f', true);
                    case 'r': return new PseudoToken((USint) '\r', true);
                    case '\"': return new PseudoToken((USint) '\"', true);
                    case '\'': return new PseudoToken((USint) '\'', true);
                    case '?': return new PseudoToken((USint) '\?', true);
                    case '\\': return new PseudoToken((USint) '\\', true);
                    default: return nullptr;
                }
            }
            return new PseudoToken((USint) v[1], true);
        default:
            break;
    }

    return nullptr;

}

PseudoToken * ExpressionEvaluator::solvePriority1(int k, int max) {

    PseudoToken * operand = (PseudoToken *) expression->at(k+1);

    switch(expression->at(k)->getName()) {
        case TokenNames::nExpADD:
            return new PseudoToken( + operand->getSolvedValue(), operand->isConst());
        case TokenNames::nExpSUB:
            return new PseudoToken( - operand->getSolvedValue(), operand->isConst());
        case TokenNames::nExpNOT:
            return new PseudoToken( ~ operand->getSolvedValue(), operand->isConst());
        default: break;
    }

    return nullptr;

}

PseudoToken * ExpressionEvaluator::solvePriority2(int k, int max) {

    PseudoToken * operand1 = (PseudoToken *) expression->at(k-1);
    PseudoToken * operand2 = (PseudoToken *) expression->at(k+1);

    switch(expression->at(k)->getName()) {
        case TokenNames::nExpMUL:
            return new PseudoToken( 
                operand1->getSolvedValue() * operand2->getSolvedValue(), operand1->isConst());
        case TokenNames::nExpDIV:
            return new PseudoToken(
                 operand1->getSolvedValue() / operand2->getSolvedValue(), operand1->isConst());
        case TokenNames::nExpMOD:
            return new PseudoToken(
                operand1->getSolvedValue() % operand2->getSolvedValue(), operand1->isConst());
        case TokenNames::nExpAND:
            return new PseudoToken(
                operand1->getSolvedValue() & operand2->getSolvedValue(), operand1->isConst());
        default: break;
    }

    return nullptr;

}

PseudoToken * ExpressionEvaluator::solvePriority3(int k, int max) {
    
    PseudoToken * operand1 = (PseudoToken *) expression->at(k-1);
    PseudoToken * operand2 = (PseudoToken *) expression->at(k+1);

    switch(expression->at(k)->getName()) {
        case TokenNames::nExpADD:
            return new PseudoToken( 
                operand1->getSolvedValue() + operand2->getSolvedValue(), operand1->isConst());
        case TokenNames::nExpSUB:
            return new PseudoToken(
                 operand1->getSolvedValue() - operand2->getSolvedValue(), operand1->isConst());
        case TokenNames::nExpOR:
            return new PseudoToken( 
                operand1->getSolvedValue() | operand2->getSolvedValue(), operand1->isConst());
        case TokenNames::nExpXOR:
            return new PseudoToken(
                 operand1->getSolvedValue() ^ operand2->getSolvedValue(), operand1->isConst());
        default: break;
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
                operand1->getSolvedValue() == operand2->getSolvedValue() ? 0b1111111111111111 : 0, 
                operand1->isConst());
        case TokenNames::nExpNE:
            return new PseudoToken(
                operand1->getSolvedValue() != operand2->getSolvedValue() ? 0b1111111111111111 : 0, 
                operand1->isConst());
        case TokenNames::nExpLT:
            return new PseudoToken(
                operand1->getSolvedValue() < operand2->getSolvedValue() ? 0b1111111111111111 : 0, 
                operand1->isConst());
        case TokenNames::nExpLE:
            return new PseudoToken(
                operand1->getSolvedValue() <= operand2->getSolvedValue() ? 0b1111111111111111 : 0, 
                operand1->isConst());
        case TokenNames::nExpGT:
            return new PseudoToken(
                operand1->getSolvedValue() > operand2->getSolvedValue() ? 0b1111111111111111 : 0, 
                operand1->isConst());
        case TokenNames::nExpGE:
            return new PseudoToken(
                operand1->getSolvedValue() >= operand2->getSolvedValue() ? 0b1111111111111111 : 0, 
                operand1->isConst());
        default: break;
    }

    return nullptr;

}

USint ExpressionEvaluator::getValue() {
    return ((PseudoToken *) expression->at(0))->getSolvedValue();
}

bool ExpressionEvaluator::isConst() {
    return ((PseudoToken *) expression->at(0))->isConst();
}

bool ExpressionEvaluator::couldNotSymbolBeResolved() {
    return symbolCouldNotBeResolved;
}
