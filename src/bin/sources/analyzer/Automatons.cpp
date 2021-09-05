#include "Automatons.hpp"




bool Automatons::qBegin_Operator() {
    Automatons::Transition * transitions[] = {new Automatons::Transition(Automatons::scanner->isAlpha(), Automatons::q1_Operator)};
    return Automatons::scanner->q(transitions, 1);
}

bool Automatons::q1_Operator() {
    Automatons::Transition * transitions[] = {new Automatons::Transition(Automatons::scanner->isAlpha(), Automatons::q2_Operator)};
    return Automatons::scanner->q(transitions, 1);
}

bool Automatons::q2_Operator() {
    Automatons::Transition * transitions[] = {new Automatons::Transition(Automatons::scanner->isAlpha(), Automatons::qEnd_Operator)};
    return Automatons::scanner->q(transitions, 1);
}

bool Automatons::qEnd_Operator() {
    LexiconScanner::TokenTypes conditions[] = {LexiconScanner::TokenTypes::OPERATOR};
    return Automatons::scanner->qEnd(new Automatons::TransitionEnd(conditions, 1));
}


Automatons::Transition::Transition(std::function<bool()> state) {
    this->condition = true;
    this->state = state;
    this->push = nullptr;
    this->pop = nullptr;
    this->stackBeEmpty = false;
}

Automatons::Transition::Transition(bool condition, std::function<bool()> state, bool stackBeEmpty) {
    this->condition = condition;
    this->state = state;
    this->push = nullptr;
    this->pop = nullptr;
    this->stackBeEmpty = stackBeEmpty;
}

Automatons::Transition::Transition(bool condition, std::function<bool()> state, 
    std::string * push=nullptr, std::string * pop=nullptr) {
    this->condition = condition;
    this->state = state;
    this->push = push;
    this->pop = pop;
    this->stackBeEmpty = false;
}



Automatons::TransitionEnd::TransitionEnd(LexiconScanner::TokenTypes * conditions, int conditionLength, Automatons::TransitionEnd::DefaultAction * defaultAction) {
    this->conditions = conditions;
    this->conditionLength = conditionLength;
    this->defaultAction = defaultAction;
    this->nonDefaultAction = new Automatons::TransitionEnd::DefaultAction(LexiconScanner::TokenTypes::NULL_TYPE, true);
}

Automatons::TransitionEnd::TransitionEnd(LexiconScanner::TokenTypes * conditions, int conditionLength, 
LexiconScanner::TokenTypes tokenType=LexiconScanner::TokenTypes::NULL_TYPE, 
bool deterministic=false, TransitionEnd::DefaultAction * defaultAction=nullptr) {
    this->conditions = conditions;
    this->conditionLength = conditionLength;
    this->defaultAction = defaultAction;
    this->nonDefaultAction = new Automatons::TransitionEnd::DefaultAction(tokenType, deterministic);
}

LexiconScanner::TokenTypes * Automatons::TransitionEnd::getConditions() {
    return this->conditions;
}

int Automatons::TransitionEnd::getConditionLength() {
    return this->conditionLength;
}

Automatons::TransitionEnd::DefaultAction * Automatons::TransitionEnd::getDefaultAction() {
    return this->defaultAction;
}

bool Automatons::TransitionEnd::isDeterministic() {
    return this->nonDefaultAction->isDeterministic();
}

LexiconScanner::TokenTypes Automatons::TransitionEnd::getTokenType() {
    return this->nonDefaultAction->getTokenType();
}

Automatons::TransitionEnd::DefaultAction::DefaultAction(LexiconScanner::TokenTypes tokenType, bool deterministic=false) {
    this->tokenType = tokenType;
    this->deterministic = deterministic;
}

LexiconScanner::TokenTypes Automatons::TransitionEnd::DefaultAction::getTokenType() {
    return this->tokenType;
}

bool Automatons::TransitionEnd::DefaultAction::isDeterministic() {
    return this->deterministic;
}


/*
    Condição da transição, só muda de estado quando satisfeita
*/
bool Automatons::Transition::getCondition() {
    return this->condition;
}
/*
    Estado para qual leva a transição caso a condição seja satifeita
*/
std::function<bool()> Automatons::Transition::getState() {
    return this->state;
}
/*
    Informa que deve além da condição ser satisfeita, a pilha deve desempilhar um conteúdo específico
*/
std::string * Automatons::Transition::whatPop() {
    return this->pop;
}
/*
    Informa que caso a condição seja satisfeita, a pilha deve empilhar um conteúdo específico
*/
std::string * Automatons::Transition::whatPush() {
    return this->push;
}
/*
    Informa que além da condição ser satisfeita, a pilha deve estar vazia
*/
bool Automatons::Transition::shouldStackBeEmpty() {
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