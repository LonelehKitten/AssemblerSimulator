#include "sources/InterfaceBus.h"

/**
 * Inicializa as configurações da InterfaceBus
 * @param handler de eventos
 */
void init(const Nan::FunctionCallbackInfo<v8::Value> & info) {
    InterfaceBus::getInstance().init(info);
}

// =======================================================
//                  SERVIÇOS DE EXECUÇÃO
// =======================================================

/**
 * Requisita expansão de macro
 * @param instruções em string
 */
void requestExpandMacros(const Nan::FunctionCallbackInfo<v8::Value> & info) {
    InterfaceBus::getInstance().serviceExpandMacros(info[0]);
}
/**
 * Requisita montagem e execução direta
 * @param instruções em string
 * @param 128Kb de memória em um array de int
 */
void requestAssembleAndRun(const Nan::FunctionCallbackInfo<v8::Value> & info) {
    InterfaceBus::getInstance().serviceAssembleAndRun(info[0], info[1]);
}
/**
 * Requisita montagem e execução passo a passo
 * @param instruções em string
 * @param 128Kb de memória em um array de int
 */
void requestAssembleAndRunBySteps(const Nan::FunctionCallbackInfo<v8::Value> & info) {
    InterfaceBus::getInstance().serviceAssembleAndRunBySteps(info[0], info[1]);
}
/**
 * Requisita execução direta
 * @param bytecode em string
 * @param 128Kb de memória em um array de int
 */
void requestRun(const Nan::FunctionCallbackInfo<v8::Value> & info) {
    InterfaceBus::getInstance().serviceRun(info[0], info[1]);
}
/**
 * Requisita execução passo a passo
 * @param bytecode em string
 * @param 128Kb de memória em um array de int
 */
void requestRunBySteps(const Nan::FunctionCallbackInfo<v8::Value> & info) {
    InterfaceBus::getInstance().serviceRunBySteps(info[0], info[1]);
}


// =======================================================
//                  SERVIÇOS AUXILIARES
// =======================================================

/**
 * Requisita execução do próximo passo
 * Utilizado junto dos serviços AssembleAndRunBySteps e RunBySteps.
 */
void requestNextStep(const Nan::FunctionCallbackInfo<v8::Value> & info) {
    InterfaceBus::getInstance().serviceNextStep();
}

/**
 * Requisita mudança no clock do processador
 * @param frequencia em int
 */
void requestClockChange(const Nan::FunctionCallbackInfo<v8::Value> & info) {
    InterfaceBus::getInstance().serviceClockChange(info[0]);
}

/**
 * Requisita parada forçada da execução
 */
void requestKillProcess(const Nan::FunctionCallbackInfo<v8::Value> & info) {
    InterfaceBus::getInstance().serviceKillProcess();
}

/**
 * Requisita envio de input para o Z808.
 * @param texto em string
 */
void requestSendInput(const Nan::FunctionCallbackInfo<v8::Value> & info) {
    InterfaceBus::getInstance().serviceSendInput(info[0]);
}


// Init
void moduleExports(v8::Local<v8::Object> exports) {

  v8::Local<v8::Context> context = exports->CreationContext();
  exports->Set(context,
               Nan::New("init").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(init)
                   ->GetFunction(context)
                   .ToLocalChecked());

  exports->Set(context,
               Nan::New("requestExpandMacros").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(requestExpandMacros)
                   ->GetFunction(context)
                   .ToLocalChecked());

  exports->Set(context,
               Nan::New("requestAssembleAndRun").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(requestAssembleAndRun)
                   ->GetFunction(context)
                   .ToLocalChecked());

  exports->Set(context,
               Nan::New("requestAssembleAndRunBySteps").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(requestAssembleAndRunBySteps)
                   ->GetFunction(context)
                   .ToLocalChecked());

  exports->Set(context,
               Nan::New("requestRun").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(requestRun)
                   ->GetFunction(context)
                   .ToLocalChecked());

  exports->Set(context,
               Nan::New("requestRunBySteps").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(requestRunBySteps)
                   ->GetFunction(context)
                   .ToLocalChecked());

  exports->Set(context,
               Nan::New("requestNextStep").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(requestNextStep)
                   ->GetFunction(context)
                   .ToLocalChecked());

  exports->Set(context,
               Nan::New("requestClockChange").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(requestClockChange)
                   ->GetFunction(context)
                   .ToLocalChecked());

  exports->Set(context,
               Nan::New("requestKillProcess").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(requestKillProcess)
                   ->GetFunction(context)
                   .ToLocalChecked());

}

NODE_MODULE(ASMR, moduleExports)
