#include "Automatons.hpp"




bool Automatons::qBegin_Operator() {
    return true;
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
    return this->whatPop;
}
/*
    Informa que caso a condição seja satisfeita, a pilha deve empilhar um conteúdo específico
*/
std::string * Automatons::Transition::whatPush() {
    return this->whatPush;
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