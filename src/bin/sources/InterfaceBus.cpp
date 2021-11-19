#include "InterfaceBus.h"

InterfaceBus::InterfaceBus() : eventEmitter(nullptr),
                               info(nullptr),
                               recognitionManager(new RecognitionManager()),
                               assembler(nullptr),
                               machine(new Z808Machine()),
                               running(false),
                               waiting(false),
                               updating(false),
                               inputing(false),
                               nextStepRequested(false),
                               service(Service::NONE)
{
    inputReport.ready = false;
    inputReport.clock = 8;
    outputReport.ready = false;
}

void InterfaceBus::init()
{
    running = true;
    producerThread = new std::thread(startProducer);
}

void InterfaceBus::finish()
{
    if (service != Service::NONE)
    {
    }
    setRunning(false);
    setWaiting(false);
    producerThread->join();
    delete producerThread;
}

void startProducer()
{
    InterfaceBus::getInstance().producer();
}

void InterfaceBus::producer()
{
    while (running)
    {
        std::this_thread::sleep_for(1s);
        setWaiting(true);
        switch (service)
        {
        case Service::EXPAND_MACROS:
            serviceThread = new std::thread(ServiceBus::startExpandMacros);
            break;
        case Service::BUILD_AND_RUN:
            serviceThread = new std::thread(ServiceBus::startBuildAndRun);
        break;
        case Service::ASSEMBLE_AND_RUN:
            serviceThread = new std::thread(ServiceBus::startAssembleAndRun);
            break;
        case Service::ASSEMBLE_AND_RUN_BY_STEPS:
            serviceThread = new std::thread(ServiceBus::startAssembleAndRunBySteps);
            break;
        case Service::RUN:
            std::cout << "primary thread: service run start" << std::endl;
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

        if (!isWaiting())
            continue;

        service = Service::NONE;
        while (isWaiting() && isRunning())
            ;
        std::this_thread::sleep_for(250ms);
        dispatchLog("thread primaria liberada", LogStatus::INFO);
        serviceThread->join();
        delete serviceThread;
    }
}

bool InterfaceBus::isRunning()
{
    bool running;
    mutex.lock();
    running = this->running;
    mutex.unlock();
    return running;
}

void InterfaceBus::setRunning(bool running)
{
    mutex.lock();
    this->running = running;
    mutex.unlock();
}

bool InterfaceBus::isWaiting()
{
    bool waiting;
    mutex.lock();
    waiting = this->waiting;
    mutex.unlock();
    return waiting;
}

void InterfaceBus::setWaiting(bool waiting)
{
    mutex.lock();
    this->waiting = waiting;
    mutex.unlock();
}

bool InterfaceBus::isUpdating()
{
    bool updating;
    mutex.lock();
    updating = this->updating;
    mutex.unlock();
    return updating;
}

void InterfaceBus::setUpdating(bool updating)
{
    mutex.lock();
    this->updating = updating;
    mutex.unlock();
}

bool InterfaceBus::isInputing()
{
    bool inputing;
    mutex.lock();
    inputing = this->inputing;
    mutex.unlock();
    return inputing;
}

void InterfaceBus::setInputing(bool inputing)
{
    mutex.lock();
    this->inputing = inputing;
    mutex.unlock();
}

bool InterfaceBus::isNextStepRequested()
{
    bool nextStepRequested;
    mutex.lock();
    nextStepRequested = this->nextStepRequested;
    mutex.unlock();
    return nextStepRequested;
}

void InterfaceBus::setNextStepRequested(bool nextStepRequested)
{
    mutex.lock();
    this->nextStepRequested = nextStepRequested;
    mutex.unlock();
}

//---

bool InterfaceBus::isOutputReady()
{
    bool outputReady;
    mutex.lock();
    outputReady = this->outputReport.ready;
    mutex.unlock();
    return outputReady;
}

void InterfaceBus::setOutputReady(bool outputReady)
{
    mutex.lock();
    this->outputReport.ready = outputReady;
    mutex.unlock();
}

bool InterfaceBus::isInputReady()
{
    bool inputReady;
    mutex.lock();
    inputReady = this->inputReport.ready;
    mutex.unlock();
    return inputReady;
}

void InterfaceBus::setInputReady(bool inputReady)
{
    mutex.lock();
    this->inputReport.ready = inputReady;
    mutex.unlock();
}

std::mutex &InterfaceBus::getMutex()
{
    std::mutex &mutex = this->mutex;
    return mutex;
}

void InterfaceBus::runExpandMacros()
{
    std::vector<Semantic *> *semantics = recognitionManager->analyzeFile(inputReport.code);
    assembler = new Assembler(semantics);
    assembler->init(false);
    delete assembler;
}

void InterfaceBus::runBuildAndRun(){  //*****************
    std::cout << "build and run" << std::endl;
    std::vector<std::string> * files = &inputReport.files;

    Assembler * assembler;
    std::vector<Assembler *> listAssemblers = std::vector<Assembler *>();
    for(unsigned int i = 0;i < files->size(); i++){
        std::vector<Semantic *> *semantics = recognitionManager->analyzeFile(files->at(i));
        assembler = new Assembler(semantics);
        listAssemblers.emplace_back(assembler);
        assembler->init(true);
        if(assembler->assemble(inputReport.modeAssembler) == 0) {
        
        }
        // assembler
    }
    //Linker
}

void InterfaceBus::runAssembleAndRun()  //*****************
{
    if(inputReport.modeAssembler == 1){ // Executa o build and run, se modeAssembler = 1
        runBuildAndRun();
        return;
    }
    std::cout << "assemble and run: begin : code:\n" << inputReport.code << std::endl;
    std::vector<Semantic *> *semantics = recognitionManager->analyzeText(inputReport.code);
    LOG("assemble and run: analyzer")
    std::this_thread::sleep_for(this->getClock()*20);
    //return;
    assembler = new Assembler(semantics);
    assembler->init(true);
    LOG("assemble and run: preprocess")
    std::this_thread::sleep_for(this->getClock()*20);

    if(assembler->assemble(inputReport.modeAssembler) == 0) {

        std::cout << "assemble and run: assemble" << std::endl;

        machine->resetMachine();
        machine->memoryUpdate(&inputReport.memory, assembler->getAssemblyCode());
        machine->setStartProgram(assembler->getStartProgram(), assembler->getStartSegment());
        LOG(std::string("Start Program = ") + std::to_string(assembler->getStartProgram()))
        machine->run(false);
        std::cout << "assemble and run: run" << std::endl;

        while (isUpdating());
        setWaiting(false);

    }

    delete assembler;
}

void InterfaceBus::runAssembleAndRunBySteps()
{

    while (isUpdating());
    setWaiting(false);
}

void InterfaceBus::runRun()
{
    machine->resetMachine();
    machine->memoryUpdate(&inputReport.memory, &inputReport.bytecode);
    machine->run(false);

    while (isUpdating());
    setWaiting(false);
}

void InterfaceBus::runRunBySteps()
{

    machine->memoryUpdate(&inputReport.memory, &inputReport.bytecode);
    machine->run(true);

    while (isUpdating())
        ;
    setWaiting(false);
}

void InterfaceBus::runTest()
{

    for (int i = 0; i < 10; i++)
    {
        dispatchLog("teste info", LogStatus::INFO);
        dispatchLog("teste error", LogStatus::ERROR);
        dispatchLog("teste success", LogStatus::SUCCESS);
        std::this_thread::sleep_for(1s);
    }
    // service = Service::NONE;
    setWaiting(false);
}

EventEmitter InterfaceBus::getEventEmitter()
{
    return (*info)[0].As<v8::Function>();
}

V8Context InterfaceBus::getV8Context()
{
    return info->GetIsolate()->GetCurrentContext();
}

// =======================================================
//                  DESPACHANTES DE EVENTOS
// =======================================================
// pra expansão de macro
void InterfaceBus::dispatchMacroExpanded(std::string code)
{
    outputReport.code = code;
    setOutputReady(true);
}

// pra cada ciclo do processador
void InterfaceBus::dispatchProgramToMemory(std::vector<byte> *memory)
{
    std::cout << "dpm: before cast" << std::endl;
    outputReport.memory = memory;
    std::cout << "dpm: after cast" << std::endl;
    setUpdating(true);
    setOutputReady(true);
}

// pra cada ciclo do processador
void InterfaceBus::dispatchCycle(Z808Response &response, bool waitingForInput, bool isBySteps)
{
    outputReport.response = response.toJSON();
    std::cout << "interfaceBus: post toJson" << std::endl;
    setUpdating(true);
    setInputing(waitingForInput);
    setNextStepRequested(!isBySteps);
    setOutputReady(true);
    std::cout << "interfaceBus: post " << std::endl;
}

// para sinalizar fim de execução
void InterfaceBus::dispatchHalt()
{
    outputReport.response = "halt";
    setUpdating(true);
    setOutputReady(true);
}

// quando houver alguma mensagem a ser printada no console
void InterfaceBus::dispatchLog(std::string message, LogStatus status)
{
    JSON logMessage = std::string("{\"message\": \"") + message +
                      std::string("\", \"status\": ") + std::to_string((int)status) +
                      std::string("}");
    logMessages.push(logMessage);
}

// =======================================================
//          VERIFICADORES DE EVENTOS DESPACHADOS
// =======================================================
void InterfaceBus::checkMacroExpanded(NodeInfo *info)
{
    this->info = info;
    if (!isOutputReady())
        return;
    trigger("macroExpanded", outputReport.code.c_str());
    setOutputReady(false);
    setWaiting(false);
}

void InterfaceBus::checkProgramToMemory(NodeInfo *info)
{
    this->info = info;
    if (!isOutputReady())
        return;
    trigger("programToMemory", castByteArraytoV8(outputReport.memory));
    std::cout << "cpm: post trigger" << std::endl;
    setOutputReady(false);
    std::cout << "cpm: post ready" << std::endl;
    setUpdating(false);
}

void InterfaceBus::checkCycle(NodeInfo *info)
{
    this->info = info;
    if (!isOutputReady())
        return;
    trigger("cycle", outputReport.response);
    std::cout << "cpm: post trigger" << std::endl;
    setOutputReady(false);
    setUpdating(false);
    std::cout << "cpm: post updating" << std::endl;
}

void InterfaceBus::checkLog(NodeInfo *info)
{
    this->info = info;
    if (logMessages.empty())
        return;
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
void InterfaceBus::serviceExpandMacros(NodeInfo *info, V8Var code)
{
    this->info = info;
    std::cout << "vai atribuir o code" << std::endl;
    inputReport.code = castV8toString(code);
    std::cout << "vai atribuir o service" << std::endl;
    service = Service::EXPAND_MACROS;
}

/**
 * Constroi e execução direta
 * @param arquivos em array de string
 * @param 128Kb de memória em um array de int
 */
void InterfaceBus::serviceBuildAndRun(NodeInfo *info, V8Var files, V8Var memory)
{
    this->info = info;
    
    //std::cout << "Request: " << castV8toString(code) << std::endl;
    inputReport.files = castV8toStringArray(files);
    inputReport.memory = castV8toByteArray(memory);
    service = Service::BUILD_AND_RUN;
}

/**
 * Montagem e execução direta
 * @param instruções em string
 * @param 128Kb de memória em um array de int
 */
void InterfaceBus::serviceAssembleAndRun(NodeInfo *info, V8Var code, V8Var memory)
{
    this->info = info;
    //std::cout << "Request: " << castV8toString(code) << std::endl;
    inputReport.code = castV8toString(code);
    inputReport.memory = castV8toByteArray(memory);
    service = Service::ASSEMBLE_AND_RUN;
}

/**
 * Montagem e execução passo a passo
 * @param instruções em string
 */
void InterfaceBus::serviceAssembleAndRunBySteps(NodeInfo *info, V8Var code, V8Var memory)
{
    this->info = info;
    inputReport.code = castV8toString(code);
    inputReport.memory = castV8toByteArray(memory);
    service = Service::ASSEMBLE_AND_RUN_BY_STEPS;
}

/**
 * Requisita execução direta
 * @param bytecode em string
 */
void InterfaceBus::serviceRun(NodeInfo *info, V8Var bytecode, V8Var memory)
{
    this->info = info;
    inputReport.bytecode = castV8toByteArray(bytecode);
    inputReport.memory = castV8toByteArray(memory);
    service = Service::RUN;
}

/**
 * Requisita execução passo a passo
 * @param bytecode em string
 */
void InterfaceBus::serviceRunBySteps(NodeInfo *info, V8Var bytecode, V8Var memory)
{
    this->info = info;
    inputReport.bytecode = castV8toByteArray(bytecode);
    inputReport.memory = castV8toByteArray(memory);
    service = Service::RUN_BY_STEPS;
}

// test
void InterfaceBus::serviceTest()
{
    service = Service::TEST;
}

// =======================================================
//                  SERVIÇOS AUXILIARES
// =======================================================
/**
 * Requisita execução do próximo passo
 * Utilizado junto dos serviços AssembleAndRunBySteps e RunBySteps.
 */
void InterfaceBus::serviceNextStep()
{
    setNextStepRequested(true);
}

/**
 * Requisita mudança no clock do processador
 * @param frequencia em int
 */
void InterfaceBus::serviceClockChange(NodeInfo *info, V8Var clock)
{
    this->info = info;
    mutex.lock();
    inputReport.clock = castV8toInt(clock);
    mutex.unlock();
}

/**
* Requisita mudança no modo do Montador
* @param tipo em int
*/
void InterfaceBus::serviceModeAssembler(NodeInfo *info, V8Var tipo)
{
    this->info = info;
    mutex.lock();
    inputReport.modeAssembler = castV8toInt(tipo);
    mutex.unlock();
}

/**
 * Requisita parada forçada da execução
 */
void InterfaceBus::serviceKillProcess()
{
    machine->forceStop();
}

/**
 * Envio de input para o Z808.
 * @param texto em string
 */
void InterfaceBus::serviceSendInput(NodeInfo *info, V8Var input)
{
    this->info = info;
    machine->setInput(castV8toString(input));
    setInputing(false);
    setNextStepRequested(true);
}

void InterfaceBus::trigger(char *event, std::string data)
{
    v8::Local<v8::Value> arguments[2] = {
        Nan::New(event).ToLocalChecked(),
        Nan::New(data).ToLocalChecked()};
    Nan::AsyncResource resource("nan:makeCallback");
    Nan::MaybeLocal<v8::Value> maybe = resource.runInAsyncScope(
        Nan::GetCurrentContext()->Global(),
        (*info)[0].As<v8::Function>(),
        2,
        arguments);
}

void InterfaceBus::trigger(char *event, v8::Local<v8::Array> data)
{
    v8::Local<v8::Value> arguments[2] = {
        Nan::New(event).ToLocalChecked(),
        data};
    Nan::AsyncResource resource("nan:makeCallback");
    Nan::MaybeLocal<v8::Value> maybe = resource.runInAsyncScope(
        Nan::GetCurrentContext()->Global(),
        (*info)[0].As<v8::Function>(),
        2,
        arguments);
}

std::string InterfaceBus::castV8toString(V8Var jsString)
{
    v8::Isolate *isolate = info->GetIsolate();
    v8::String::Utf8Value str(isolate, jsString);
    return std::string(*str);
}

int InterfaceBus::castV8toInt(V8Var jsNumber)
{
    return (int)jsNumber->NumberValue(info->GetIsolate()->GetCurrentContext()).FromJust();
}
std::vector<std::string> InterfaceBus::castV8toStringArray(V8Var jsNumberArray){
 
    v8::Local<v8::Array> jsArray = v8::Local<v8::Array>::Cast(jsNumberArray);   
    unsigned int length = (unsigned int)jsArray->Length();
    std::string *array = (std::string *)malloc(sizeof(std::string) * length);
    for (unsigned int i = 0; i < length; i++)
    {
        array[i] = (std::string)castV8toString(Nan::Get(jsArray, i).ToLocalChecked());
    }
    return std::vector<std::string>(array, array + length);
}

std::vector<byte> InterfaceBus::castV8toByteArray(V8Var jsNumberArray)
{
    v8::Local<v8::Array> jsArray = v8::Local<v8::Array>::Cast(jsNumberArray);
    unsigned int length = (unsigned int)jsArray->Length();
    byte *array = (byte *)malloc(sizeof(byte) * length);
    for (unsigned int i = 0; i < length; i++)
    {
        array[i] = (byte)castV8toInt(Nan::Get(jsArray, i).ToLocalChecked());
    }
    return std::vector<byte>(array, array + length);
}

v8::Local<v8::Array> InterfaceBus::castByteArraytoV8(std::vector<byte> *array)
{
    std::cout << "dpm: 1" << std::endl;
    v8::Local<v8::Array> jsArray = Nan::New<v8::Array>();
    std::cout << "dpm: 2" << std::endl;
    for (int i = 0; i < (int)array->size(); i++)
    {
        Nan::Set(jsArray, i, Nan::New<v8::Number>(array->at(i)));
    }
    std::cout << "dpm: 3" << std::endl;
    return jsArray;
}

InterfaceBus &InterfaceBus::getInstance()
{
    static InterfaceBus nodeBus;
    return nodeBus;
}

double InterfaceBus::getMilliseconds()
{
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

milliseconds InterfaceBus::getClock()
{
    mutex.lock();
    milliseconds clock(1000 / inputReport.clock);
    mutex.unlock();
    return clock;
}
