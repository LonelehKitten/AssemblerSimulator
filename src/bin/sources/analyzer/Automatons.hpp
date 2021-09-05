#ifndef AUTOMATONS_HPP
#define AUTOMATONS_HPP

#include <string>
#include <functional>

#include "LexiconScanner.hpp"

namespace Automatons {

        LexiconScanner * scanner;

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
                std::function<bool()> state;
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
                Transition(std::function<bool()> state);
                Transition(bool condition, std::function<bool()> state, bool stackBeEmpty);
                Transition(bool condition, std::function<bool()> state, std::string * push=nullptr, std::string * pop=nullptr);
                /*
                    Condição da transição, só muda de estado quando satisfeita
                */
                bool getCondition();
                /*
                    Estado para qual leva a transição caso a condição seja satifeita
                */
                std::function<bool()> getState();
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
                        LexiconScanner::TokenTypes tokenType;
                        bool deterministic;
                    public:
                        DefaultAction(LexiconScanner::TokenTypes tokenType, bool deterministic=false);
                        LexiconScanner::TokenTypes getTokenType();
                        bool isDeterministic();
                };
            private:
                LexiconScanner::TokenTypes * conditions;
                int conditionLength;
                TransitionEnd::DefaultAction * defaultAction, * nonDefaultAction;
            public:
                TransitionEnd(LexiconScanner::TokenTypes * conditions, int conditionLength, TransitionEnd::DefaultAction * defaultAction);
                TransitionEnd(LexiconScanner::TokenTypes * conditions, int conditionLength, LexiconScanner::TokenTypes tokenType, 
                    bool deterministic=false, TransitionEnd::DefaultAction * defaultAction=nullptr);
                LexiconScanner::TokenTypes * getConditions();
                int getConditionLength();
                TransitionEnd::DefaultAction * getDefaultAction();
                bool isDeterministic();
                LexiconScanner::TokenTypes getTokenType();
        };

        bool qBegin_Operator();
        bool q1_Operator();
        bool q2_Operator();
        bool qEnd_Operator();

};

#endif /* AUTOMATONS_HPP */