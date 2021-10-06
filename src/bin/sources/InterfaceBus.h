#ifndef INTERFACEBUS_H
#define INTERFACEBUS_H

#include <nan.h>

#include <chrono>
#include <iostream>
#include <thread>
#include <string>

#include "analyzer/RecognitionManager.h"
#include "assembler/Assembler.h"
#include "machine/Z808Machine.h"
#include "machine/Z808Response.h"

using namespace std::chrono;

typedef v8::Local<v8::Function> EventEmitter;
typedef Nan::FunctionCallbackInfo<v8::Value> NodeInfo;

enum LogStatus {
    INFO = 0,
    ERROR,
    SUCCESS
};

class InterfaceBus {
    private:
         EventEmitter * eventEmitter;
         NodeInfo * info;

         RecognitionManager * recognitionManager;
         Assembler * assembler;
         Z808Machine * machine;

         InterfaceBus();

         std::string trigger(std::string event, std::string data);
         std::string castV8String(v8::Value& jsString);

    public:

         InterfaceBus(InterfaceBus const&) = delete;
         void operator=(InterfaceBus const&) = delete;

         static InterfaceBus& getInstance();

         // =======================================================
         //                  DESPACHANTES DE EVENTOS
         // =======================================================
         /*
            Retorna o código com as macros expandidas.
            Deve ser chamado pelo Assembler em caso de sucesso
         */
         void dispatchMacroExpanded(std::string code); // pra expansão de macro
         /*
            Retorna o estado dos registradores no ciclo corrente bem como
            comandos de entrada e saída de dados, e mudanças na memória.
            Deve ser executado pelo Z808Machine caso a instrução seja válida.
         */
         std::string dispatchCycle(Z808Response& response);         // pra cada ciclo do processador
         /*
            Retorna uma mensagem no console da interface.
            A mensagem pode ser de log, erro ou sucesso.
            Ver LogStatus
         */
         void dispatchLog(std::string message, LogStatus status);           // quando houver alguma mensagem a ser printada no console


         // =======================================================
         //                  SERVIÇOS DE EXECUÇÃO
         // =======================================================
         /**
          * Expansão de macro
          * @param instruções em string
          */
         void serviceExpandMacros(std::string code);
         /**
          * Montagem e execução direta
          * @param instruções em string
          * @param 128Kb de memória em um array de int
          */
         void serviceAssembleAndRun(std::string code, char * memory);
         /**
          * Montagem e execução passo a passo
          * @param instruções em string
          */
         void serviceAssembleAndRunBySteps(std::string code, char * memory);
         /**
          * Requisita execução direta
          * @param bytecode em string
          */
         void serviceRun(std::string bytecode, char * memory);
         /**
          * Requisita execução passo a passo
          * @param bytecode em string
          */
         void serviceRunBySteps(std::string bytecode,  char * memory);

         // =======================================================
         //                  SERVIÇOS AUXILIARES
         // =======================================================
         /**
          * Requisita execução do próximo passo
          * Utilizado junto dos serviços AssembleAndRunBySteps e RunBySteps.
          */
         void serviceNextStep();

         /**
          * Requisita mudança no clock do processador
          * @param frequencia em int
          */
         void serviceClockChange(int clock);

         /**
          * Requisita parada forçada da execução
          */
         void serviceKillProcess();


         EventEmitter * getEventEmitter();
         void setEventEmitter(EventEmitter *newEventEmitter);
         NodeInfo * getInfo();
         void setInfo(NodeInfo *newInfo);

         double getMilliseconds();

};

#endif // INTERFACEBUS_H
