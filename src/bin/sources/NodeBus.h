#ifndef NODEBUS_H
#define NODEBUS_H

#include <nan.h>

#include <chrono>
#include <iostream>
#include <thread>
#include <string>

#include "sources/analyzer/RecognitionManager.h"
#include "sources/assembler/Assembler.h"

typedef v8::Local<v8::Function> EventEmitter;
typedef Nan::FunctionCallbackInfo<v8::Value> NodeInfo;

class NodeBus {
    private:
         EventEmitter eventEmitter;
         NodeInfo info;

         RecognitionManager * recognitionManager;
         Assembler * assembler;

    public:
        NodeBus(EventEmitter& eventEmitter, NodeInfo& info);

        void onMacroExpanded(); // pra expansão de macro
        void onCycle();         // pra cada ciclo do processador
        void onError();
};

static NodeBus * nodeBus = nullptr;

#endif // NODEBUS_H
