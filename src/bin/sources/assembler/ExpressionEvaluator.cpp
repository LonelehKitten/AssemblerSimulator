#include "ExpressionEvaluator.h"

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

*/
void ExpressionEvaluator::solve(int precedenceBegin) {

    for(int i = precedenceBegin; i < (int) expression->size(); i++) {
        if(expression->at(i)->getType() == TokenTypes::tExpPRECEDENCE_OP) {
            solve(i);
        }
    }

}

void ExpressionEvaluator::solvePriority1() {

}

void ExpressionEvaluator::solvePriority2() {

}

void ExpressionEvaluator::solvePriority3() {

}

void ExpressionEvaluator::solvePriority4() {
    
}

USint ExpressionEvaluator::getValue() {
    return value;
}