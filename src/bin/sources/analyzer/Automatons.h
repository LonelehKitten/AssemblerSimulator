#ifndef AUTOMATONS_H
#define AUTOMATONS_H

#include <string>
#include <functional>

#include "Dictionaries.h"

class LexiconScanner;

namespace Automatons {

        /*
            Classe que representa a transição de um estado para outro mediante a satisfação
            de uma condição específica.
        */
        class Transition {
            private:
                /*
                    Condição da transição, só muda de estado quando satisfeita
                */
                bool condition;
                /*
                    Estado para qual leva a transição caso a condição seja satifeita
                */
                std::function<bool(LexiconScanner *)> state;
                /*
                    Informa que deve além da condição ser satisfeita, a pilha deve desempilhar um conteúdo específico
                */
                std::string * pop;
                /*
                    Informa que caso a condição seja satisfeita, a pilha deve empilhar um conteúdo específico
                */
                std::string * push;
                /*
                    Informa que além da condição ser satisfeita, a pilha deve estar vazia
                */
                bool stackBeEmpty;
            public:
                Transition(std::function<bool(LexiconScanner *)> state);
                Transition(bool condition, std::function<bool(LexiconScanner *)> state, bool stackBeEmpty);
                Transition(bool condition, std::function<bool(LexiconScanner *)> state, std::string * push=nullptr, std::string * pop=nullptr);
                /*
                    Condição da transição, só muda de estado quando satisfeita
                */
                bool getCondition();
                /*
                    Estado para qual leva a transição caso a condição seja satifeita
                */
                std::function<bool(LexiconScanner *)> getState();
                /*
                    Informa que deve além da condição ser satisfeita, a pilha deve desempilhar um conteúdo específico
                */
                std::string * whatPop();
                /*
                    Informa que caso a condição seja satisfeita, a pilha deve empilhar um conteúdo específico
                */
                std::string * whatPush();
                /*
                    Informa que além da condição ser satisfeita, a pilha deve estar vazia
                */
                bool shouldStackBeEmpty();
        };

        class TransitionEnd {
            public:
                class DefaultAction {
                    private:
                        TokenTypes tokenType;
                        bool deterministic;
                    public:
                        DefaultAction(TokenTypes tokenType, bool deterministic=true);
                        TokenTypes getTokenType();
                        bool isDeterministic();
                };
            private:
                TokenTypes * conditions;
                int conditionLength;
                TransitionEnd::DefaultAction * defaultAction, * nonDefaultAction;
            public:
                TransitionEnd(TokenTypes * conditions, int conditionLength, TransitionEnd::DefaultAction * defaultAction);
                TransitionEnd(TokenTypes * conditions, int conditionLength,
                              TokenTypes tokenType=TokenTypes::tNULL_TYPE,
                              bool deterministic=false, TransitionEnd::DefaultAction * defaultAction=nullptr);
                TokenTypes * getConditions();
                int getConditionLength();
                TransitionEnd::DefaultAction * getDefaultAction();
                bool isDeterministic();
                TokenTypes getTokenType();
        };

        bool qBegin_Operator(LexiconScanner * scanner);
        bool q1_Operator(LexiconScanner * scanner);
        bool q2_Operator(LexiconScanner * scanner);
        bool qEnd_Operator(LexiconScanner * scanner);

};


#endif /* AUTOMATONS_H */