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

console.log("Module C++: ", asmr != null);
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
    console.log(data);
});

const getEmitter = () => emitter.emit.bind(emitter);

LogEventObserver = setInterval(() => asmr?.observeLogFiring(getEmitter()), 10);

const memoryChanges = [];
for (let b = 0; b < 128; b++) {
    for (let i = 0; i < 32; i++) {
        for (let j = 0; j < 16; j++) {
            memoryChanges.push(parseInt(0).toString('16').padStart(4, 0));
        }
    }
}

const run = () => {
    /*
    asmr.requestAssembleAndRun(`
ADD AX, 1022
ADD AX, AX
SUB AX, 511
SUB AX, 511
JZ 9
SUB AX, 511
JMP -14
INT 2
    `, memoryChanges);*/
    asmr.requestAssembleAndRun(`
Dados SEGMENT
    Mem1 DW ?
    Mem2 DW ?
Dados ENDS
Codigo SEGMENT
ASSUME CS:Codigo
ASSUME DS:Dados
Inicio:
mov Mem1, AX
Codigo ENDS
END Inicio
`, memoryChanges);
    ProgramToMemoryEventObserver = setInterval(() => asmr.observeProgramToMemoryFiring(getEmitter()), 10);
}

run();