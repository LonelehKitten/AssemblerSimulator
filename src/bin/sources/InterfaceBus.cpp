#include "InterfaceBus.h"

InterfaceBus::InterfaceBus() :
    eventEmitter(nullptr),
    info(nullptr),
    recognitionManager(new RecognitionManager()),
    assembler(nullptr),
    machine(new Z808Machine()),
    running(false),
    waiting(false),
    updating(false),
    service(Service::NONE)
{
    inputReport.ready = false;
    inputReport.clock = 8;
    outputReport.ready = false;
}

void InterfaceBus::init() {
    running = true;
    producerThread = new std::thread(startProducer);
}

void InterfaceBus::finish() {
    if(service != Service::NONE) {

    }
    setRunning(false);
    setWaiting(false);
    producerThread->join();
    delete producerThread;
}

void startProducer() {
    InterfaceBus::getInstance().producer();
}

void InterfaceBus::producer() {
    while(running) {
        std::this_thread::sleep_for(1s);
        setWaiting(true);
        switch(service) {
            case Service::EXPAND_MACROS:
                serviceThread = new std::thread(ServiceBus::startExpandMacros);
                break;
            case Service::ASSEMBLE_AND_RUN:
                serviceThread = new std::thread(ServiceBus::startAssembleAndRun);
                break;
            case Service::ASSEMBLE_AND_RUN_BY_STEPS:
                serviceThread = new std::thread(ServiceBus::startAssembleAndRunBySteps);
                break;
            case Service::RUN:
                serviceThread = new std::thread(ServiceBus::startRun);
                break;
            case Service::RUN_BY_STEPS:
                serviceThread = new std::thread(ServiceBus::startRunBySteps);
                break;
            case Service::TEST:
                serviceThread = new std::thread(ServiceBus::startTest);
                break;
            default:
                setWaiting(false);
        }

        if(!isWaiting()) continue;

        service = Service::NONE;
        while(isWaiting() && isRunning());
        std::this_thread::sleep_for(250ms);
        dispatchLog("thread primaria liberada", LogStatus::INFO);
        serviceThread->join();
        delete serviceThread;
    }
}

bool InterfaceBus::isRunning() {
    bool running;
    mutex.lock();
    running = this->running;
    mutex.unlock();
    return running;
}

void InterfaceBus::setRunning(bool running) {
    mutex.lock();
    this->running = running;
    mutex.unlock();
}

bool InterfaceBus::isWaiting() {
    bool waiting;
    mutex.lock();
    waiting = this->waiting;
    mutex.unlock();
    return waiting;
}

void InterfaceBus::setWaiting(bool waiting) {
    mutex.lock();
    this->waiting = waiting;
    mutex.unlock();
}

bool InterfaceBus::isUpdating() {
    bool updating;
    mutex.lock();
    updating = this->updating;
    mutex.unlock();
    return updating;
}

void InterfaceBus::setUpdating(bool updating) {
    mutex.lock();
    this->updating = updating;
    mutex.unlock();
}

bool InterfaceBus::isInputing() {
    bool inputing;
    mutex.lock();
    inputing = this->inputing;
    mutex.unlock();
    return inputing;
}

void InterfaceBus::setInputing(bool inputing) {
    mutex.lock();
    this->inputing = inputing;
    mutex.unlock();
}

void InterfaceBus::runExpandMacros() {
    std::vector<Semantic *> * semantics = recognitionManager->analyze(inputReport.code, false);
    assembler = new Assembler(semantics);
    assembler->init(false);
    delete assembler;
}

void InterfaceBus::runAssembleAndRun() {

    while(isUpdating());
    setWaiting(false);
}

void InterfaceBus::runAssembleAndRunBySteps() {

    while(isUpdating());
    setWaiting(false);
}

void InterfaceBus::runRun() {

    while(isUpdating());
    setWaiting(false);
}

void InterfaceBus::runRunBySteps() {

    while(isUpdating());
    setWaiting(false);
}

void InterfaceBus::runTest() {

    for(int i = 0; i < 10; i++) {
        dispatchLog("teste info", LogStatus::INFO);
        dispatchLog("teste error", LogStatus::ERROR);
        dispatchLog("teste success", LogStatus::SUCCESS);
        std::this_thread::sleep_for(1s);
    }
    //service = Service::NONE;
    setWaiting(false);
}

EventEmitter InterfaceBus::getEventEmitter() {
    return (*info)[0].As<v8::Function>();
}

V8Context InterfaceBus::getV8Context() {
    return info->GetIsolate()->GetCurrentContext();
}

// =======================================================
//                  DESPACHANTES DE EVENTOS
// =======================================================
// pra expansão de macro
void InterfaceBus::dispatchMacroExpanded(std::string code) {
    outputReport.code = code;
    outputReport.ready = true;
}

// pra cada ciclo do processador
void InterfaceBus::dispatchCycle(Z808Response& response) {
    outputReport.response = response.toJSON();
    outputReport.ready = true;
}

// para sinalizar fim de execução
void InterfaceBus::dispatchHalt() {
    outputReport.response = "halt";
    outputReport.ready = true;
}

// quando houver alguma mensagem a ser printada no console
void InterfaceBus::dispatchLog(std::string message, LogStatus status) {
    std::string logMessage = std::string("{\"message\": \"") + message +
                            std::string("\", \"status\": ") + std::to_string((int) status) +
                            std::string("}");
    logMessages.push(logMessage);
}

// =======================================================
//          VERIFICADORES DE EVENTOS DESPACHADOS
// =======================================================
void InterfaceBus::checkMacroExpanded(NodeInfo * info) {
    this->info = info;
    if(!outputReport.ready) return;
    trigger("macroExpanded", outputReport.code);
    outputReport.ready = false;
    setWaiting(false);
}

void InterfaceBus::checkCycle(NodeInfo * info) {
    this->info = info;
    if(!outputReport.ready) return;
    trigger("cycle", outputReport.response);
    outputReport.ready = false;
    setUpdating(false);
}

void InterfaceBus::checkLog(NodeInfo * info) {
    this->info = info;
    if(logMessages.empty()) return;
    trigger("log", logMessages.front());
    logMessages.pop();
}

// =======================================================
//                  SERVIÇOS DE EXECUÇÃO
// =======================================================
/**
 * Expansão de macro
 * @param instruções em string
 */
void InterfaceBus::serviceExpandMacros(NodeInfo * info, V8Var code) {
    this->info = info;
    std::cout << "vai atribuir o code" << std::endl;
    inputReport.code = castV8toString(code);
    std::cout << "vai atribuir o service" << std::endl;
    service = Service::EXPAND_MACROS;
}

/**
 * Montagem e execução direta
 * @param instruções em string
 * @param 128Kb de memória em um array de int
 */
void InterfaceBus::serviceAssembleAndRun(NodeInfo * info, V8Var code, V8Var memory) {
    this->info = info;
    inputReport.code = castV8toString(code);
    inputReport.memory = castV8toByteArray(memory);
    service = Service::ASSEMBLE_AND_RUN;
}

/**
 * Montagem e execução passo a passo
 * @param instruções em string
 */
void InterfaceBus::serviceAssembleAndRunBySteps(NodeInfo * info, V8Var code, V8Var memory) {
    this->info = info;
    inputReport.code = castV8toString(code);
    inputReport.memory = castV8toByteArray(memory);
    service = Service::ASSEMBLE_AND_RUN_BY_STEPS;
}

/**
 * Requisita execução direta
 * @param bytecode em string
 */
void InterfaceBus::serviceRun(NodeInfo * info, V8Var bytecode, V8Var memory) {
    this->info = info;
    inputReport.bytecode = castV8toByteArray(bytecode);
    inputReport.memory = castV8toByteArray(memory);
    service = Service::RUN;
}

/**
 * Requisita execução passo a passo
 * @param bytecode em string
 */
void InterfaceBus::serviceRunBySteps(NodeInfo * info, V8Var bytecode,  V8Var memory) {
    this->info = info;
    inputReport.bytecode = castV8toByteArray(bytecode);
    inputReport.memory = castV8toByteArray(memory);
    service = Service::RUN_BY_STEPS;
}

// test
void InterfaceBus::serviceTest() {
    service = Service::TEST;
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
    mutex.lock();
    inputReport.clock = castV8toInt(clock);
    mutex.unlock();
}

/**
 * Requisita parada forçada da execução
 */
void InterfaceBus::serviceKillProcess() {

}

/**
 * Envio de input para o Z808.
 * @param texto em string
 */
void InterfaceBus::serviceSendInput(V8Var input) {

}


void InterfaceBus::trigger(char * event, std::string data) {
    v8::Local<v8::Value> arguments[2] = {
      Nan::New(event).ToLocalChecked(),
      Nan::New(data).ToLocalChecked()
    };
    Nan::AsyncResource resource("nan:makeCallback");
    Nan::MaybeLocal<v8::Value> maybe = resource.runInAsyncScope(
                Nan::GetCurrentContext()->Global(),
                (*info)[0].As<v8::Function>(),
                2,
                arguments
            );

    //if(!maybe.IsEmpty()) return castV8toString(maybe.ToLocalChecked());

    //return "";
}

std::string InterfaceBus::castV8toString(V8Var jsString) {
  v8::Isolate* isolate = info->GetIsolate();
  v8::String::Utf8Value str(isolate, jsString);
  return std::string(*str);
}

int InterfaceBus::castV8toInt(V8Var jsNumber) {
    return (int) jsNumber->NumberValue(info->GetIsolate()->GetCurrentContext()).FromJust();
}

char * InterfaceBus::castV8toByteArray(V8Var jsNumberArray) {
    v8::Local<v8::Array> jsArray = v8::Local<v8::Array>::Cast(jsNumberArray);
    unsigned int length = (unsigned int) jsArray->Length();
    char * array = (char *) malloc(sizeof(char)*length);
    for(unsigned int i = 0; i < length; i++) {
        array[i] = (char) castV8toInt(Nan::Get(jsArray, i).ToLocalChecked());
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

milliseconds InterfaceBus::getClock() {
    mutex.lock();
    milliseconds clock(1000/inputReport.clock);
    mutex.unlock();
    return clock;
}
