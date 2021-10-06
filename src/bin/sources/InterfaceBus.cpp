#include "InterfaceBus.h"

InterfaceBus::InterfaceBus() :
    eventEmitter(nullptr),
    info(nullptr),
    recognitionManager(new RecognitionManager()),
    assembler(nullptr),
    machine(new Z808Machine())
{}



// pra expansão de macro
void InterfaceBus::dispatchMacroExpanded(std::string code) {
    trigger("macroExpanded", code);
}

// pra cada ciclo do processador
std::string InterfaceBus::dispatchCycle(Z808Response& response) {
    return trigger("cycle", response.toJSON());
}

// quando houver alguma mensagem a ser printada no console
void InterfaceBus::dispatchLog(std::string message, LogStatus status) {
    trigger("log",
        std::string("{message: \"") + message +
        std::string("\", status: ") + std::to_string((int) status) + std::string("}")
    );
}

// =======================================================
//                  SERVIÇOS DE EXECUÇÃO
// =======================================================
/**
 * Expansão de macro
 * @param instruções em string
 */
void InterfaceBus::serviceExpandMacros(std::string code) {
    std::vector<Semantic *> * semantics = recognitionManager->analyze(code, false);
    assembler = new Assembler(semantics);
    assembler->init(false);
    delete assembler;
}

/**
 * Montagem e execução direta
 * @param instruções em string
 * @param 128Kb de memória em um array de int
 */
void InterfaceBus::serviceAssembleAndRun(std::string code, char * memory) {

}

/**
 * Montagem e execução passo a passo
 * @param instruções em string
 */
void InterfaceBus::serviceAssembleAndRunBySteps(std::string code, char * memory) {

}

/**
 * Requisita execução direta
 * @param bytecode em string
 */
void InterfaceBus::serviceRun(std::string bytecode, char * memory) {

}

/**
 * Requisita execução passo a passo
 * @param bytecode em string
 */
void InterfaceBus::serviceRunBySteps(std::string bytecode,  char * memory) {

}

// =======================================================
//                  SERVIÇOS AUXILIARES
// =======================================================
/**
 * Requisita execução do próximo passo
 * Utilizado junto dos serviços AssembleAndRunBySteps e RunBySteps.
 */
void InterfaceBus::serviceNextStep() {

}

/**
 * Requisita mudança no clock do processador
 * @param frequencia em int
 */
void InterfaceBus::serviceClockChange(int clock) {

}

/**
 * Requisita parada forçada da execução
 */
void InterfaceBus::serviceKillProcess() {

}


std::string InterfaceBus::trigger(std::string event, std::string data) {
    v8::Local<v8::Value> arguments[2] = {
      Nan::New(event).ToLocalChecked(),
      Nan::New(data).ToLocalChecked()
    };
    Nan::AsyncResource resource("nan:makeCallback");
    return castV8String(
                resource.runInAsyncScope(
                        Nan::GetCurrentContext()->Global(),
                        eventEmitter,
                        2,
                        arguments
                    ).ToLocalChecked()
                );
}

std::string InterfaceBus::castV8String(v8::Value& jsString) {
  v8::Isolate* isolate = info->GetIsolate();
  v8::String::Utf8Value str(isolate, jsString);
  return std::string(*str);
}

InterfaceBus& InterfaceBus::getInstance() {

    static InterfaceBus nodeBus;
    return nodeBus;
}

EventEmitter * InterfaceBus::getEventEmitter() {
    return eventEmitter;
}

void InterfaceBus::setEventEmitter(EventEmitter * newEventEmitter) {
    eventEmitter = newEventEmitter;
}

NodeInfo * InterfaceBus::getInfo() {
    return info;
}

void InterfaceBus::setInfo(NodeInfo * newInfo) {
    info = newInfo;
}

double InterfaceBus::getMilliseconds() {
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}
