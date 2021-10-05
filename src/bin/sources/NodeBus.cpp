#include "NodeBus.h"

NodeBus::NodeBus() :
    eventEmitter(nullptr),
    info(nullptr),
    recognitionManager(new RecognitionManager()),
    assembler(nullptr),
    machine(new Z808Machine())
{}



// pra expansão de macro
void NodeBus::onMacroExpanded(std::string code) {
    trigger("macroExpanded", code);
}

// pra cada ciclo do processador
std::string NodeBus::onCycle(Z808Response& response) {
    return trigger("cycle", response.toJSON());
}

// quando houver alguma mensagem a ser printada no console
void NodeBus::onLog(std::string message, LogStatus status) {
    trigger("log",
        std::string("{message: \"") + message +
        std::string("\", status: ") + std::to_string((int) status) + std::string("}")
    );
}

std::string NodeBus::trigger(std::string event, std::string data) {
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

std::string NodeBus::castV8String(v8::Value& jsString) {
  v8::Isolate* isolate = info->GetIsolate();
  v8::String::Utf8Value str(isolate, jsString);
  return std::string(*str);
}

NodeBus& NodeBus::getInstance() {

    static NodeBus nodeBus;
    return nodeBus;
}

EventEmitter * NodeBus::getEventEmitter() {
    return eventEmitter;
}

void NodeBus::setEventEmitter(EventEmitter * newEventEmitter) {
    eventEmitter = newEventEmitter;
}

NodeInfo * NodeBus::getInfo() {
    return info;
}

void NodeBus::setInfo(NodeInfo * newInfo) {
    info = newInfo;
}
