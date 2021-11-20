#include "sources/InterfaceBus.h"

/**
 * Inicializa as configurações da InterfaceBus
 * @param handler de eventos
 */
void init(const Nan::FunctionCallbackInfo<v8::Value> & info) {
    InterfaceBus::getInstance().init();
}

void finish(const Nan::FunctionCallbackInfo<v8::Value> & info) {
    InterfaceBus::getInstance().finish();
}

// =======================================================
//                  SERVIÇOS DE EXECUÇÃO
// =======================================================

/**
 * Requisita expansão de macro
 * @param instruções em string
 */
void requestExpandMacros(const Nan::FunctionCallbackInfo<v8::Value> & info) {
    std::cout << "cpp: begin" << std::endl;
    InterfaceBus::getInstance().serviceExpandMacros((NodeInfo *) &info, info[0]);
    std::cout << "cpp: end" << std::endl;
}

/**
 * Requisita construção e execução direta
 * @param files em array de string
 * @param 128Kb de memória em um array de int
 */
void requestBuildAndRun(const Nan::FunctionCallbackInfo<v8::Value> & info) {
    InterfaceBus::getInstance().serviceBuildAndRun((NodeInfo *) &info, info[0], info[1]);
}
/**
 * Requisita montagem e execução direta
 * @param instruções em string
 * @param 128Kb de memória em um array de int
 */
void requestAssembleAndRun(const Nan::FunctionCallbackInfo<v8::Value> & info) {
    InterfaceBus::getInstance().serviceAssembleAndRun((NodeInfo *) &info, info[0], info[1]);
}
/**
 * Requisita montagem e execução passo a passo
 * @param instruções em string
 * @param 128Kb de memória em um array de int
 */
void requestAssembleAndRunBySteps(const Nan::FunctionCallbackInfo<v8::Value> & info) {
    InterfaceBus::getInstance().serviceAssembleAndRunBySteps((NodeInfo *) &info, info[0], info[1]);
}
/**
 * Requisita execução direta
 * @param bytecode em string
 * @param 128Kb de memória em um array de int
 */
void requestRun(const Nan::FunctionCallbackInfo<v8::Value> & info) {
    std::cout << "cpp: begin" << std::endl;
    InterfaceBus::getInstance().serviceRun((NodeInfo *) &info, info[0], info[1]);
    std::cout << "cpp: end" << std::endl;
}
/**
 * Requisita execução passo a passo
 * @param bytecode em string
 * @param 128Kb de memória em um array de int
 */
void requestRunBySteps(const Nan::FunctionCallbackInfo<v8::Value> & info) {
    InterfaceBus::getInstance().serviceRunBySteps((NodeInfo *) &info, info[0], info[1]);
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
    InterfaceBus::getInstance().serviceClockChange((NodeInfo *) &info, info[0]);
}

/**
 * Requisita mudança no modo do Montador
 * @param tipo em int
 */
void requestModeAssembler(const Nan::FunctionCallbackInfo<v8::Value> & info) {
    InterfaceBus::getInstance().serviceModeAssembler((NodeInfo *) &info, info[0]);
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
    InterfaceBus::getInstance().serviceSendInput((NodeInfo *) &info, info[0]);
}

// =======================================================
//            OBSERVADORES DE DISPARO DE EVENTOS
// =======================================================

void observeExpandedMacrosFiring(const Nan::FunctionCallbackInfo<v8::Value> & info) {
    InterfaceBus::getInstance().checkMacroExpanded((NodeInfo *) &info);
}

void observeCycleFiring(const Nan::FunctionCallbackInfo<v8::Value> & info) {
    InterfaceBus::getInstance().checkCycle((NodeInfo *) &info);
}

void observeLogFiring(const Nan::FunctionCallbackInfo<v8::Value> & info) {
    InterfaceBus::getInstance().checkLog((NodeInfo *) &info);
}

void observeProgramToMemoryFiring(const Nan::FunctionCallbackInfo<v8::Value> & info) {
    InterfaceBus::getInstance().checkProgramToMemory((NodeInfo *) &info);
}

// TEST
void requestTest(const Nan::FunctionCallbackInfo<v8::Value> & info) {
    InterfaceBus::getInstance().serviceTest();
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
               Nan::New("finish").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(finish)
                   ->GetFunction(context)
                   .ToLocalChecked());

  exports->Set(context,
               Nan::New("requestExpandMacros").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(requestExpandMacros)
                   ->GetFunction(context)
                   .ToLocalChecked());
  exports->Set(context,
      Nan::New("requestSendInput").ToLocalChecked(),
      Nan::New<v8::FunctionTemplate>(requestSendInput)
      ->GetFunction(context)
      .ToLocalChecked());

  exports->Set(context,
               Nan::New("requestBuildAndRun").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(requestBuildAndRun)
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
               Nan::New("requestTest").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(requestTest)
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
               Nan::New("requestModeAssembler").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(requestModeAssembler)
                   ->GetFunction(context)
                   .ToLocalChecked());

  exports->Set(context,
               Nan::New("requestKillProcess").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(requestKillProcess)
                   ->GetFunction(context)
                   .ToLocalChecked());

  exports->Set(context,
               Nan::New("observeExpandedMacrosFiring").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(observeExpandedMacrosFiring)
                   ->GetFunction(context)
                   .ToLocalChecked());

  exports->Set(context,
               Nan::New("observeCycleFiring").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(observeCycleFiring)
                   ->GetFunction(context)
                   .ToLocalChecked());

  exports->Set(context,
               Nan::New("observeLogFiring").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(observeLogFiring)
                   ->GetFunction(context)
                   .ToLocalChecked());

  exports->Set(context,
               Nan::New("observeProgramToMemoryFiring").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(observeProgramToMemoryFiring)
                   ->GetFunction(context)
                   .ToLocalChecked());

}

NODE_MODULE(ASMR, moduleExports)
