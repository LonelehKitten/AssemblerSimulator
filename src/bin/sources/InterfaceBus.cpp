#include "InterfaceBus.h"

InterfaceBus::InterfaceBus() :
    eventEmitter(nullptr),
    info(nullptr),
    recognitionManager(new RecognitionManager()),
    assembler(nullptr),
    machine(new Z808Machine())
{}

void InterfaceBus::init(NodeInfo& info) {
    this->info = &info;
    this->eventEmitter = &info[0].As<v8::Function>();
    this->context = &info->GetIsolate()->GetCurrentContext();
}

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
void InterfaceBus::serviceExpandMacros(V8Var code) {
    std::vector<Semantic *> * semantics = recognitionManager->analyze(castV8toString(code), false);
    assembler = new Assembler(semantics);
    assembler->init(false);
    delete assembler;
}

/**
 * Montagem e execução direta
 * @param instruções em string
 * @param 128Kb de memória em um array de int
 */
void InterfaceBus::serviceAssembleAndRun(V8Var code, V8Var memory) {

}

/**
 * Montagem e execução passo a passo
 * @param instruções em string
 */
void InterfaceBus::serviceAssembleAndRunBySteps(V8Var code, V8Var memory) {

}

/**
 * Requisita execução direta
 * @param bytecode em string
 */
void InterfaceBus::serviceRun(V8Var bytecode, V8Var memory) {

}

/**
 * Requisita execução passo a passo
 * @param bytecode em string
 */
void InterfaceBus::serviceRunBySteps(V8Var bytecode,  V8Var memory) {

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
void InterfaceBus::serviceClockChange(V8Var clock) {

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

std::string InterfaceBus::castV8toString(V8Var& jsString) {
  v8::Isolate* isolate = info->GetIsolate();
  v8::String::Utf8Value str(isolate, jsString);
  return std::string(*str);
}

int InterfaceBus::castV8toInt(V8Var& jsNumber) {
    return (int) jsNumber->NumberValue(*context).FromJust();
}

char * InterfaceBus::castV8toByteArray(V8Var& jsNumberArray) {
    v8::Local<v8::Array> jsArray = v8::Local<v8::Array>::Cast(jsNumberArray);
    unsigned int length = (unsigned int) jsArray->Get(v8::String::New("length"))->ToObject()->Uint32Value();
    char * array = (char *) malloc(sizeof(char)*length);
    for(unsigned int i = 0; i < length; i++) {
        array[i] = (char) castV8toInt(jsArray->Get(i));
    }
    return array;
}

InterfaceBus& InterfaceBus::getInstance() {

    static InterfaceBus nodeBus;
    return nodeBus;
}

double InterfaceBus::getMilliseconds() {
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}
