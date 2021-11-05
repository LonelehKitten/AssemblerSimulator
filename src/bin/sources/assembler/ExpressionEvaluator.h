#ifndef EXPRESSION_EVALUATOR_H
#define EXPRESSION_EVALUATOR_H

#include <vector>
#include <string>
#include <unordered_map>
#include "Symbol.h"
#include "../Enums.h"

class PseudoToken : public Token {

    private:
        USint resolvedValue;

    public:
        PseudoToken(USint resolvedValue);
};

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


    ORDEM DE PRIORIDADE
    0. '(' ')'
    1. 'not' '+'(unario) '-' (unário)
    2. '*' '/' 'mod' 'and'
    3. '+' '-' 'or' 'xor'
    4. 'eq' 'ne' 'lt' 'le' 'gt' 'ge'

*/
class ExpressionEvaluator {
    private:
        bool priority1, priority2, priority3, priority4;
        Expression * expression;
        SymbolTable * symbolTable;

        USint value;

        void solve();

        void solvePriority1();
        void solvePriority2();
        void solvePriority3();
        void solvePriority4();

    public:
        ExpressionEvaluator(Expression * expression, SymbolTable * symbolTable);

        USint getValue();
};

#endif /* EXPRESSION_EVALUATOR_H */