#ifndef INTERFACEBUS_H
#define INTERFACEBUS_H

#include <nan.h>

#include <chrono>
#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <queue>
#include <vector>
#include <cstdlib>

#include "analyzer/RecognitionManager.h"
#include "assembler/Assembler.h"
#include "machine/Z808Machine.h"
#include "machine/Z808Response.h"
#include "ServiceBus.h"

using namespace std::chrono;
using namespace std::literals::chrono_literals;

typedef v8::Local<v8::Function> EventEmitter;
typedef Nan::FunctionCallbackInfo<v8::Value> NodeInfo;
typedef v8::Local<v8::Value> V8Var;
typedef v8::Local<v8::Context> V8Context;

typedef unsigned char byte;

enum LogStatus {
    INFO = 0,
    ERROR,
    SUCCESS
};

enum Service {
    NONE,
    EXPAND_MACROS,
    ASSEMBLE_AND_RUN,
    ASSEMBLE_AND_RUN_BY_STEPS,
    RUN,
    RUN_BY_STEPS,
    TEST
};

typedef struct InputReport {
    bool ready;
    std::string code;
    std::vector<byte> bytecode;
    std::vector<byte> memory;
    std::string input;
    int clock;
} InputReport;

typedef struct OutputReport {
    bool ready;
    JSON  response;
    std::string code;
    JSON  memory;
} OutputReport;

class InterfaceBus {
    private:
         EventEmitter * eventEmitter;
         NodeInfo * info;
         V8Context * context;

         RecognitionManager * recognitionManager;
         Assembler * assembler;
         Z808Machine * machine;

         std::thread * producerThread, * serviceThread;
         std::mutex mutex;

         bool running, waiting, updating, inputing, nextStepRequested;
         Service service;

         InputReport inputReport;
         OutputReport outputReport;
         std::queue<JSON> logMessages;

         InterfaceBus();

         void trigger(char * event, std::string data);

         std::string castV8toString(V8Var jsString);
         int castV8toInt(V8Var jsNumber);
         std::vector<byte> castV8toByteArray(V8Var jsNumberArray);
         JSON castByteArraytoJSON(std::vector<byte> * array);

         EventEmitter getEventEmitter();
         V8Context getV8Context();

         void setRunning(bool running);
         void setWaiting(bool waiting);
         void setUpdating(bool updating);
         void setInputing(bool inputing);
         void setNextStepRequested(bool nextStepRequested);

    public:

         bool isRunning();
         bool isWaiting();
         bool isUpdating();
         bool isInputing();
         bool isNextStepRequested();
         std::mutex& getMutex();

         InterfaceBus(InterfaceBus const&) = delete;
         void operator=(InterfaceBus const&) = delete;

         static InterfaceBus& getInstance();

         void init();
         void finish();
         void producer();

         // =======================================================
         //                  DESPACHANTES DE EVENTOS
         // =======================================================
         /*
            Retorna o código com as macros expandidas.
            Deve ser chamado pelo Assembler em caso de sucesso
         */
         void dispatchMacroExpanded(std::string code); // pra expansão de macro

         void dispatchProgramToMemory(std::vector<byte> * memory);
         /*
            Retorna o estado dos registradores no ciclo corrente bem como
            comandos de entrada e saída de dados, e mudanças na memória.
            Deve ser executado pelo Z808Machine caso a instrução seja válida.
         */
         void dispatchCycle(Z808Response& response, bool waitingForInput);         // pra cada ciclo do processador
         void dispatchHalt(); // sinaliza fim de execução do processador
         /*
            Retorna uma mensagem no console da interface.
            A mensagem pode ser de log, erro ou sucesso.
            Ver LogStatus
         */
         void dispatchLog(std::string message, LogStatus status);           // quando houver alguma mensagem a ser printada no console

         // =======================================================
         //          VERIFICADORES DE EVENTOS DESPACHADOS
         // =======================================================
         void checkMacroExpanded(NodeInfo * info);
         void checkProgramToMemory(NodeInfo * info);
         void checkCycle(NodeInfo * info);
         void checkLog(NodeInfo * info);

         // =======================================================
         //                  SERVIÇOS DE EXECUÇÃO
         // =======================================================
         /**
          * Expansão de macro
          * @param instruções em string
          */
         void serviceExpandMacros(NodeInfo * info, V8Var code);
         /**
          * Montagem e execução direta
          * @param instruções em string
          * @param 128Kb de memória em um array de int
          */
         void serviceAssembleAndRun(NodeInfo * info, V8Var code, V8Var memory);
         /**
          * Montagem e execução passo a passo
          * @param instruções em string
          */
         void serviceAssembleAndRunBySteps(NodeInfo * info, V8Var code, V8Var memory);
         /**
          * Execução direta
          * @param bytecode em string
          */
         void serviceRun(NodeInfo * info, V8Var bytecode, V8Var memory);
         /**
          * Execução passo a passo
          * @param bytecode em string
          */
         void serviceRunBySteps(NodeInfo * info, V8Var bytecode,  V8Var memory);

         // =======================================================
         //                  SERVIÇOS AUXILIARES
         // =======================================================
         /**
          * Execução do próximo passo
          * Utilizado junto dos serviços AssembleAndRunBySteps e RunBySteps.
          */
         void serviceNextStep();

         /**
          * Parada forçada da execução
          */
         void serviceKillProcess();

         /**
          * Mudança no clock do processador
          * @param frequencia em int
          */
         void serviceClockChange(V8Var clock);

         /**
          * Envio de input para o Z808.
          * @param texto em string
          */
         void serviceSendInput(V8Var input);

         // test
         void serviceTest();

         void runExpandMacros();

         void runAssembleAndRun();

         void runAssembleAndRunBySteps();

         void runRun();

         void runRunBySteps();

         void runTest();



         double getMilliseconds();
         milliseconds getClock();
};

void startProducer();

#endif // INTERFACEBUS_H
