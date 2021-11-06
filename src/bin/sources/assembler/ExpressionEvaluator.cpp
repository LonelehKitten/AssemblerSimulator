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
    symbolTable(symbolTable)
{

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
void ExpressionEvaluator::solve(int precedenceBegin) {

    int precedenceEnd = -1;
    USint value;

    for(int i = precedenceBegin; i < (int) expression->size(); i++) {
        if(expression->at(i)->getType() == TokenTypes::tExpPRECEDENCE_OP) {
            solve(i);
            continue;
        }
        if(expression->at(i)->getType() == TokenTypes::tExpPRECEDENCE_ED) {
            precedenceEnd = i;
            break;
        }
    }

    int i = precedenceBegin+1;
    while(precedenceBegin+2 < precedenceEnd) {

        if(!priority1) {
            if(
                    expression->at(i)->getName() == TokenNames::nExpADD ||
                    expression->at(i)->getName() == TokenNames::nExpSUB ||
                    expression->at(i)->getName() == TokenNames::nExpNOT
            ) {
                if(i-1 >= 0 && !isOperand(expression->at(i-1)) && isOperand(expression->at(i+1))) {
                    value = solvePriority1(i, precedenceEnd);
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
                    value = solvePriority2(i, precedenceEnd);
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
                    value = solvePriority3(i, precedenceEnd);
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
                    value = solvePriority4(i, precedenceEnd);
                }
            }
        }

        i++;

        if(i == (int) expression->size() && !priority1) {
            priority1 = true;
            i = precedenceBegin;
        } else if(i == (int) expression->size() && !priority2) {
            priority2 = true;
            i = precedenceBegin;
        } else if(i == (int) expression->size() && !priority3) {
            priority3 = true;
            i = precedenceBegin;
        } else if(i == (int) expression->size() && !priority4) {
            priority4 = true;
            i = precedenceBegin;
        }

    }

}

bool ExpressionEvaluator::isOperand(Token * token) {
    return (
        token->getType() == TokenTypes::tIDENTIFIER ||
        token->getType() == TokenTypes::tNULL_TYPE ||
        token->getType() == TokenTypes::tDECIMAL ||
        token->getType() == TokenTypes::tHEXADECIMAL ||
        token->getType() == TokenTypes::tBINARY ||
        token->getType() == TokenTypes::tBINARY
    );
}

PseudoToken * ExpressionEvaluator::solveSymbol(Token * token) {

    switch(token->getType()) {
        case TokenTypes::tIDENTIFIER:
            if(symbolTable->find(token->getToken()) != symbolTable->end()) {
                return new PseudoToken(std::stoi(symbolTable->find(token->getToken())->second->value));
            }
            return nullptr;
        case TokenTypes::tDECIMAL:
            //USint
        case TokenTypes::tHEXADECIMAL:
            return ~ solvedValue->getSolvedValue();
        case TokenTypes::tBINARY:
        case TokenTypes::tCHARACTERE:
    }

    return 0;

}

USint ExpressionEvaluator::solvePriority1(int k, int max) {

    PseudoToken * solvedValue = (PseudoToken *) expression->at(k+1);

    switch(expression->at(k)->getName()) {
        case TokenNames::nExpADD:
            return + solvedValue->getSolvedValue();
        case TokenNames::nExpSUB:
            return - solvedValue->getSolvedValue();
        case TokenNames::nExpNOT:
            return ~ solvedValue->getSolvedValue();
    }

    return 0;

}

USint ExpressionEvaluator::solvePriority2(int k, int max) {
    return 0;
}

USint ExpressionEvaluator::solvePriority3(int k, int max) {
    return 0;
}

USint ExpressionEvaluator::solvePriority4(int k, int max) {
    return 0;
}

USint ExpressionEvaluator::getValue() {
    return value;
}
