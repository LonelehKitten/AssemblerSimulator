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
    ERRO,
    SUCCESS
};

enum Service {
    NONE,
    EXPAND_MACROS,
    BUILD_AND_RUN,
    ASSEMBLE_AND_RUN,
    ASSEMBLE_AND_RUN_BY_STEPS,
    RUN,
    RUN_BY_STEPS,
    TEST
};

typedef struct InputReport {
    bool ready;
    std::vector<std::string> * files;
    std::string code;
    std::vector<byte> bytecode;
    std::vector<byte> memory;
    std::string input;
    int clock;
    int modeAssembler;
} InputReport;

typedef struct OutputReport {
    bool ready;
    JSON  response;
    std::string code;
    std::vector<byte> * memory;
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
         void trigger(char * event, v8::Local<v8::Array> data);

         std::string castV8toString(V8Var jsString);
         std::vector<std::string> * castV8toStringArray(V8Var jsStringArray);
         
         int castV8toInt(V8Var jsNumber);
         std::vector<byte> castV8toByteArray(V8Var jsNumberArray);
         //std::vector<std::string> castV8toStringArray(V8Var jsNumberArray);
         v8::Local<v8::Array> castByteArraytoV8(std::vector<byte> * array);

         EventEmitter getEventEmitter();
         V8Context getV8Context();

         void setRunning(bool running);
         void setWaiting(bool waiting);
         void setUpdating(bool updating);
         void setInputing(bool inputing);

         bool isOutputReady();

         void setOutputReady(bool outputReady);

         bool isInputReady();

         void setInputReady(bool inputReady);

    public:

         bool isRunning();
         bool isWaiting();
         bool isUpdating();
         bool isInputing();
         bool isNextStepRequested();
         void setNextStepRequested(bool nextStepRequested);
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
            Retorna o c??digo com as macros expandidas.
            Deve ser chamado pelo Assembler em caso de sucesso
         */
         void dispatchMacroExpanded(std::string code); // pra expans??o de macro

         void dispatchProgramToMemory(std::vector<byte> * memory);
         /*
            Retorna o estado dos registradores no ciclo corrente bem como
            comandos de entrada e sa??da de dados, e mudan??as na mem??ria.
            Deve ser executado pelo Z808Machine caso a instru????o seja v??lida.
         */
         void dispatchCycle(Z808Response& response, bool waitingForInput, bool isBySteps);         // pra cada ciclo do processador
         void dispatchHalt(); // sinaliza fim de execu????o do processador
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
         //                  SERVI??OS DE EXECU????O
         // =======================================================
         /**
          * Expans??o de macro
          * @param instru????es em string
          */
         void serviceExpandMacros(NodeInfo * info, V8Var code);
         /**
        * Constroi e execu????o direta
        * @param arquivos em array de string
        * @param 128Kb de mem??ria em um array de int
        */
        void serviceBuildAndRun(NodeInfo *info, V8Var files, V8Var memory);
         /**
          * Montagem e execu????o direta
          * @param instru????es em string
          * @param 128Kb de mem??ria em um array de int
          */
         void serviceAssembleAndRun(NodeInfo * info, V8Var code, V8Var memory);
         /**
          * Montagem e execu????o passo a passo
          * @param instru????es em string
          */
         void serviceAssembleAndRunBySteps(NodeInfo * info, V8Var code, V8Var memory);
         /**
          * Execu????o direta
          * @param bytecode em string
          */
         void serviceRun(NodeInfo * info, V8Var bytecode, V8Var memory);
         /**
          * Execu????o passo a passo
          * @param bytecode em string
          */
         void serviceRunBySteps(NodeInfo * info, V8Var bytecode,  V8Var memory);

         // =======================================================
         //                  SERVI??OS AUXILIARES
         // =======================================================
         /**
          * Execu????o do pr??ximo passo
          * Utilizado junto dos servi??os AssembleAndRunBySteps e RunBySteps.
          */
         void serviceNextStep();

         /**
          * Parada for??ada da execu????o
          */
         void serviceKillProcess();

         /**
          * Mudan??a no clock do processador
          * @param frequencia em int
          */
         void serviceClockChange(NodeInfo * info, V8Var clock);

         /**
          * Requisita mudan??a no modo do Montador
          * @param tipo em int
          */
         void serviceModeAssembler(NodeInfo * info, V8Var tipo);

         /**
          * Envio de input para o Z808.
          * @param texto em string
          */
         void serviceSendInput(NodeInfo * info, V8Var input);

         // test
         void serviceTest();

         void runExpandMacros();

         void runBuildAndRun();
         
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
