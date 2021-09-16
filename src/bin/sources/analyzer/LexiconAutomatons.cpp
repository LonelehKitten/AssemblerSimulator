#include "LexiconAutomatons.h"
#include "LexiconScanner.h"

#define qEndLPCONDITIONS 15
#define qEndSPCONDITIONS 10

namespace LexiconAutomatons {

    // <labelPattern>
    bool qBegin_labelPattern(LexiconScanner * scanner) {
        Transition * transitions = new Transition(scanner->isAlpha() || scanner->is('_'), q1_labelPattern);
        return scanner->q(&transitions, 1);
    }


    bool q1_labelPattern(LexiconScanner * scanner) {
        Transition * transitions = new Transition(scanner->isAlphaNumeric() || scanner->is('_'), q1_labelPattern);
        return scanner->q(&transitions, 1, new Transition(qEnd_labelPattern));
    }


    bool qEnd_labelPattern(LexiconScanner * scanner) {
        TokenTypes * conditions = (TokenTypes *) malloc(sizeof(TokenTypes)*qEndLPCONDITIONS);
        conditions[0] = TokenTypes::tBLOCKDEF;
        conditions[1] = TokenTypes::tBLOCKEND;
        conditions[2] = TokenTypes::tEND;
        conditions[3] = TokenTypes::tOPERATION;
        conditions[4] = TokenTypes::tREGISTER;
        conditions[5] = TokenTypes::tASSUME;
        conditions[6] = TokenTypes::tVARDEF;
        conditions[7] = TokenTypes::tCONSTDEF;
        conditions[8] = TokenTypes::tExpARITHMETICb;
        conditions[9] = TokenTypes::tExpLOGICALb;
        conditions[10] = TokenTypes::tExpLOGICALu;
        conditions[11] = TokenTypes::tExpRELATIONALb;
        conditions[12] = TokenTypes::tORG;
        conditions[13] = TokenTypes::tOFFSET;
        conditions[14] = TokenTypes::tSTACK;
        return scanner->qEnd(new TransitionEnd(conditions, qEndLPCONDITIONS, false, false,
            new TransitionEnd::DefaultAction(TokenTypes::tIDENTIFIER, false)
        ));
    }
    // </labelPattern>


    // <symbolPattern>
    bool qBegin_symbolPattern(LexiconScanner * scanner) {
        Transition * transitions = new Transition(scanner->isSpecial(), qEnd_symbolPattern);
        return scanner->q(&transitions, 1);
    }


    bool qEnd_symbolPattern(LexiconScanner * scanner) {
        TokenTypes * conditions = (TokenTypes *) malloc(sizeof(TokenTypes)*qEndSPCONDITIONS);
        conditions[0] = TokenTypes::tExpARITHMETICb;
        conditions[1] = TokenTypes::tExpARITHMETICu;
        conditions[2] = TokenTypes::tExpPRECEDENCE_OP;
        conditions[3] = TokenTypes::tExpPRECEDENCE_ED;
        conditions[4] = TokenTypes::tINDEX_OP;
        conditions[5] = TokenTypes::tINDEX_ED;
        conditions[6] = TokenTypes::tPC;
        conditions[7] = TokenTypes::tSEPARATOR;
        conditions[8] = TokenTypes::tCOLON;
        conditions[9] = TokenTypes::tUNDEFINED;
        return scanner->qEnd(new TransitionEnd(conditions, qEndSPCONDITIONS, false));
    }
    // </symbolPattern>


    // <decimalPattern>
    bool qBegin_decimalLiteral(LexiconScanner * scanner) {
        Transition * transitions = new Transition(scanner->isNumeric(), q1_decimalLiteral);
        return scanner->q(&transitions, 1);
    }

    bool q1_decimalLiteral(LexiconScanner * scanner) {
        Transition ** transitions = (Transition **) malloc(sizeof(Transition *)*2);
        transitions[0] = new Transition(scanner->isNumeric(), q1_decimalLiteral);
        transitions[1] = new Transition(scanner->is('d') || scanner->is('D'), qEnd_decimalLiteral1);
        return scanner->q(transitions, 2, new Transition(qEnd_decimalLiteral2));
    }

    bool qEnd_decimalLiteral1(LexiconScanner * scanner) {
        TokenTypes conditions = TokenTypes::tNULL_TYPE;
        return scanner->qEnd(new TransitionEnd(&conditions, 1, false, true, new TransitionEnd::DefaultAction(TokenTypes::tDECIMAL)));
    }

    bool qEnd_decimalLiteral2(LexiconScanner * scanner) {
        TokenTypes conditions = TokenTypes::tNULL_TYPE;
        return scanner->qEnd(new TransitionEnd(&conditions, 1, false, false, new TransitionEnd::DefaultAction(TokenTypes::tDECIMAL, false)));
    }
    // </decimalPattern>

    // <hexadecimalPattern>
    bool qBegin_hexadecimalLiteral(LexiconScanner * scanner) {
        Transition * transitions = new Transition(
                    scanner->isNumeric() || scanner->isBetween('a', 'f') || scanner->isBetween('A', 'F'), q1_hexadecimalLiteral);
        return scanner->q(&transitions, 1);
    }

    bool q1_hexadecimalLiteral(LexiconScanner * scanner) {
        Transition ** transitions = (Transition **) malloc(sizeof(Transition *)*2);
        transitions[0] = new Transition(scanner->isNumeric() || scanner->isBetween('a', 'f') || scanner->isBetween('A', 'F'), q1_hexadecimalLiteral);
        transitions[1] = new Transition(scanner->is('h') || scanner->is('H'), qEnd_hexadecimalLiteral);
        return scanner->q(transitions, 2);
    }

    bool qEnd_hexadecimalLiteral(LexiconScanner * scanner) {
        TokenTypes conditions = TokenTypes::tNULL_TYPE;
        return scanner->qEnd(new TransitionEnd(&conditions, 1, false, true, new TransitionEnd::DefaultAction(TokenTypes::tHEXADECIMAL)));
    }
    // </hexadecimalPattern>

    // <binaryPattern>
    bool qBegin_binaryLiteral(LexiconScanner * scanner) {
        Transition * transitions = new Transition(scanner->is('0') || scanner->is('1'), q1_binaryLiteral);
        return scanner->q(&transitions, 1);
    }

    bool q1_binaryLiteral(LexiconScanner * scanner) {
        Transition ** transitions = (Transition **) malloc(sizeof(Transition *)*2);
        transitions[0] = new Transition(scanner->is('0') || scanner->is('1'), q1_binaryLiteral);
        transitions[1] = new Transition(scanner->is('b') || scanner->is('B'), qEnd_binaryLiteral);
        return scanner->q(transitions, 2);
    }

    bool qEnd_binaryLiteral(LexiconScanner * scanner) {
        TokenTypes conditions = TokenTypes::tNULL_TYPE;
        return scanner->qEnd(new TransitionEnd(&conditions, 1, false, true, new TransitionEnd::DefaultAction(TokenTypes::tBINARY)));
    }
    // </binaryPattern>

    // <charPattern>
    bool qBegin_charLiteral(LexiconScanner * scanner) {
        Transition * transitions = new Transition(scanner->is('\''), q1_charLiteral);
        return scanner->q(&transitions, 1);
    }

    bool q1_charLiteral(LexiconScanner * scanner) {
        Transition ** transitions = (Transition **) malloc(sizeof(Transition *)*2);
        transitions[0] = new Transition(scanner->is('\\'), q2_charLiteral);
        transitions[1] = new Transition(scanner->isAlphaNumeric() || scanner->isSpecial(), q3_charLiteral);
        return scanner->q(transitions, 1);
    }

    bool q2_charLiteral(LexiconScanner * scanner) {
        Transition * transitions = new Transition(scanner->isAlphaNumeric() || scanner->isSpecial(), qEnd_charLiteral);
        return scanner->q(&transitions, 1);
    }

    bool q3_charLiteral(LexiconScanner * scanner) {
        Transition * transitions = new Transition(scanner->is('\''), qEnd_charLiteral);
        return scanner->q(&transitions, 1);
    }

    bool qEnd_charLiteral(LexiconScanner * scanner) {
        TokenTypes conditions = TokenTypes::tNULL_TYPE;
        return scanner->qEnd(new TransitionEnd(&conditions, 1, false, true, new TransitionEnd::DefaultAction(TokenTypes::tCHARACTERE)));
    }
    // </charPattern>

}

LexiconAutomatons::Transition::Transition(std::function<bool(LexiconScanner *)> state) {
    this->condition = true;
    this->state = state;
    this->push = nullptr;
    this->pop = nullptr;
    this->stackBeEmpty = false;
}

LexiconAutomatons::Transition::Transition(bool condition, std::function<bool(LexiconScanner *)> state, bool stackBeEmpty) {
    this->condition = condition;
    this->state = state;
    this->push = nullptr;
    this->pop = nullptr;
    this->stackBeEmpty = stackBeEmpty;
}

LexiconAutomatons::Transition::Transition(bool condition, std::function<bool(LexiconScanner *)> state,
    std::string * push, std::string * pop) {
    this->condition = condition;
    this->state = state;
    this->push = push;
    this->pop = pop;
    this->stackBeEmpty = false;
}



LexiconAutomatons::TransitionEnd::TransitionEnd(TokenTypes * conditions, int conditionLength,  bool caseSensitive,
    bool deterministic, LexiconAutomatons::TransitionEnd::DefaultAction * defaultAction) :
    conditions(conditions), conditionLength(conditionLength),
    caseSensitive(caseSensitive),
    defaultAction(defaultAction),
    nonDefaultAction(new LexiconAutomatons::TransitionEnd::DefaultAction(TokenTypes::tNULL_TYPE, deterministic))
{}

LexiconAutomatons::TransitionEnd::TransitionEnd(TokenTypes * conditions, int conditionLength, bool caseSensitive,
bool deterministic, TokenTypes tokenType, TransitionEnd::DefaultAction * defaultAction) :
    conditions(conditions),
    conditionLength(conditionLength),
    caseSensitive(caseSensitive),
    defaultAction(defaultAction),
    nonDefaultAction(new LexiconAutomatons::TransitionEnd::DefaultAction(tokenType, deterministic))
{}

TokenTypes * LexiconAutomatons::TransitionEnd::getConditions() {
    return this->conditions;
}

int LexiconAutomatons::TransitionEnd::getConditionLength() {
    return this->conditionLength;
}

LexiconAutomatons::TransitionEnd::DefaultAction * LexiconAutomatons::TransitionEnd::getDefaultAction() {
    return this->defaultAction;
}

bool LexiconAutomatons::TransitionEnd::isDeterministic() {
    return this->nonDefaultAction->isDeterministic();
}

bool LexiconAutomatons::TransitionEnd::isCaseSensitive() {
    return this->caseSensitive;
}

TokenTypes LexiconAutomatons::TransitionEnd::getTokenType() {
    return this->nonDefaultAction->getTokenType();
}

LexiconAutomatons::TransitionEnd::DefaultAction::DefaultAction(TokenTypes tokenType, bool deterministic) {
    this->tokenType = tokenType;
    this->deterministic = deterministic;
}

TokenTypes LexiconAutomatons::TransitionEnd::DefaultAction::getTokenType() {
    return this->tokenType;
}

bool LexiconAutomatons::TransitionEnd::DefaultAction::isDeterministic() {
    return this->deterministic;
}


/*
    Condição da transição, só muda de estado quando satisfeita
*/
bool LexiconAutomatons::Transition::getCondition() {
    return this->condition;
}
/*
    Estado para qual leva a transição caso a condição seja satifeita
*/
std::function<bool(LexiconScanner *)> LexiconAutomatons::Transition::getState() {
    return this->state;
}
/*
    Informa que deve além da condição ser satisfeita, a pilha deve desempilhar um conteúdo específico
*/
std::string * LexiconAutomatons::Transition::whatPop() {
    return this->pop;
}
/*
    Informa que caso a condição seja satisfeita, a pilha deve empilhar um conteúdo específico
*/
std::string * LexiconAutomatons::Transition::whatPush() {
    return this->push;
}
/*
    Informa que além da condição ser satisfeita, a pilha deve estar vazia
*/
bool LexiconAutomatons::Transition::shouldStackBeEmpty() {
    return this->stackBeEmpty;
}

/*

#include <iostream>
#include <string>
#include <array>
#include <stack>
#include <functional>

class Transition {
    
    private:
        bool condition;
        std::string * state;
    public:
        Transition(bool condition, std::string state) {
            this->condition = condition;
            this->state = new std::string(state.c_str());
        }
        
        bool getCondition() {
            return condition;
        }
        
        std::string * getState() {
            return state;
        }
};

class Teste {
    private:
        Transition ** transitions, * defaultAction;
        int transitionsLength;
    public:
        Teste(Transition ** transitions, int transitionsLength, Transition * defaultAction) {
            this->transitions = transitions;
            this->defaultAction = defaultAction;
            this->transitionsLength = transitionsLength;
        }
        
        std::string getResult() {
            
            if(transitions != nullptr) {
                
                for(int i = 0; i < transitionsLength; i++) {
                    if(transitions[i]->getCondition())
                        return *(transitions[i]->getState());
                }
                
            }
            
            if(defaultAction != nullptr && defaultAction->getCondition())
                return *(defaultAction->getState());
            
            return "none";
        }
};

void printStdArray(auto& arr) {
    
    std::cout << "\n" << "std::array<Transition *, 3>" << "\n" << std::endl;
    
    std::cout << "0:  " << arr[0]->getState()->c_str() << std::endl;
    std::cout << "1:  " << arr[1]->getState()->c_str() << std::endl;
    std::cout << "2:  " << arr[2]->getState()->c_str() << std::endl;
    
}

void printArray(Transition ** arr) {
    std::cout << "\n" << "Transition **" << "\n" << std::endl;
    
    std::cout << "0:  " << arr[0]->getState()->c_str() << std::endl;
    std::cout << "1:  " << arr[1]->getState()->c_str() << std::endl;
    std::cout << "2:  " << arr[2]->getState()->c_str() << std::endl;
}

void run(std::function<void()> func) {
    func();
}

void printFunc() {
    std::cout << "Run function" << std::endl;
}

int main()
{
    int x = 10;
    
    std::array<Transition *, 3> arr1 = {
        new Transition(x < 10, "state1"),
        new Transition(x == 10, "state2"),
        new Transition(x > 10, "state3")
    };
    
    printStdArray(arr1);
    
    Transition * arr2[] = {
        new Transition(x > 0 && x < 10, "state1"),
        new Transition(x == 10, "state2"),
        new Transition(x > 10, "state3")
    };
    
    Teste * teste = new Teste(arr2, 3, new Transition(x == 0, "standard state"));
    
    //printArray(arr2);
    
    std::cout << std::endl;
    
    std::cout << "R:  " << teste->getResult() << std::endl;
    
    std::stack<int> * st = new std::stack<int>();
    
    std::function f = printFunc;
    
    run(f);
    
    std::string * str;// = new std::string("teste");
    
    //*(str) += '1';
    
    if(str) {
        std::cout << "is not null:  " << *str << std::endl;
    } else if (!str) {
        std::cout << "is null" << std::endl;
    }
    

    return 0;
}



*/
