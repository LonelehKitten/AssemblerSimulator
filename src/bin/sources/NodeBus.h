#ifndef NODEBUS_H
#define NODEBUS_H

#include <nan.h>

#include <chrono>
#include <iostream>
#include <thread>
#include <string>

#include "analyzer/RecognitionManager.h"
#include "assembler/Assembler.h"
#include "machine/Z808Machine.h"
#include "machine/Z808Response.h"

typedef v8::Local<v8::Function> EventEmitter;
typedef Nan::FunctionCallbackInfo<v8::Value> NodeInfo;

enum LogStatus {
    INFO = 0,
    ERROR,
    SUCCESS
};

class NodeBus {
    private:
         EventEmitter * eventEmitter;
         NodeInfo * info;

         RecognitionManager * recognitionManager;
         Assembler * assembler;
         Z808Machine * machine;

         NodeBus();

         std::string trigger(std::string event, std::string data);
         std::string castV8String(v8::Value& jsString);

    public:

         NodeBus(NodeBus const&) = delete;
         void operator=(NodeBus const&) = delete;

         static NodeBus& getInstance();

         /*
            Retorna o código com as macros expandidas.
            Deve ser chamado pelo Assembler em caso de sucesso
        */
         void onMacroExpanded(std::string code); // pra expansão de macro
         /*
            Retorna o estado dos registradores no ciclo corrente bem como
            comandos de entrada e saída de dados, e mudanças na memória.
            Deve ser executado pelo Z808Machine caso a instrução seja válida.
        */
         std::string onCycle(Z808Response& response);         // pra cada ciclo do processador
         /*
            Retorna uma mensagem no console da interface.
            A mensagem pode ser de log, erro ou sucesso.
            Ver LogStatus
        */
         void onLog(std::string message, LogStatus status);           // quando houver alguma mensagem a ser printada no console




         EventEmitter * getEventEmitter();
         void setEventEmitter(EventEmitter *newEventEmitter);
         NodeInfo * getInfo();
         void setInfo(NodeInfo *newInfo);

};

#endif // NODEBUS_H
