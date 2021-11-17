const { app } = require('electron');

app.commandLine.appendSwitch('js-flags', '--max-old-space-size=4096');

const bindings = require('bindings')
const EventEmitter = require('events');

let asmr;
try {
    asmr = bindings('ASMR') ?? null; // Pega o ASMR se ele estiver compilado
} catch (e) {
    asmr = null;
    console.log(e);
}

console.log("Module C++: ", asmr);
const isAsmr = asmr != null;
if (isAsmr) asmr.init();
const emitter = new EventEmitter();
let playing = false;
let MacroExpandedEventObserver = null;
let CycleEventObserver = null;
let LogEventObserver = null;
let ProgramToMemoryEventObserver = null;


// Eventos
emitter.on('macroExpanded', (data) => {

    console.log("macro Expand");
    clearInterval(MacroExpandedEventObserver)
    playing = false;
});
emitter.on("programToMemory", (data) => {
    const text = [];
    for (let i = 0; i < data.length; i += 2) {
        text.push((data[i].toString(16).padStart(2, 0) + data[i + 1].toString(16).padStart(2, 0)));
    }
    console.log("init memory");
    clearInterval(ProgramToMemoryEventObserver)
    CycleEventObserver = setInterval(() => asmr.observeCycleFiring(getEmitter()), 10);
});
emitter.on('cycle', (data) => {
    if (data === "halt") {
        playing = false;
        clearInterval(CycleEventObserver);
        console.log("end");
        return;
    }
    console.log('on cycle:', data);
    const response = JSON.parse(data);
    console.log(response);

});
emitter.on('log', (data) => {
    console.log(JSON.parse(data));
});

const getEmitter = () => emitter.emit.bind(emitter);

LogEventObserver = setInterval(() => asmr?.observeLogFiring(getEmitter()), 10);

const run = () => {

    asmr.requestAssembleAndRun(`
        Codigo SEGMENT
        ASSUME CS: Codigo
        ASSUME DS: Dados
        Inicio:
        ADD ax, 10
        CODIGO ENDS
        END Inicio
    `, []);
    ProgramToMemoryEventObserver = setInterval(() => asmr.observeProgramToMemoryFiring(getEmitter()), 10);
}

run();