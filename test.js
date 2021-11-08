const bindings = require('bindings')
const EventEmitter = require('events');

let asmr;
try{
    asmr = bindings('ASMR') ?? null; // Pega o ASMR se ele estiver compilado
}catch(e){
    asmr = null;
    console.log(e);
}

console.log("Module C++: ",asmr);
const isAsmr = asmr != null; 
if(isAsmr) asmr.init();
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
emitter.on("programToMemory",(data) => {
    const text= [];
    for(let i=0;i<data.length;i += 2){
        text.push((data[i].toString(16).padStart(2, 0)+data[i+1].toString(16).padStart(2, 0)));
    }
    console.log("init memory");
    clearInterval(ProgramToMemoryEventObserver)
    CycleEventObserver = setInterval(() => asmr.observeCycleFiring(getEmitter()), 10);
});
emitter.on('cycle', (data) => {
    if(data === "halt"){
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
Dados SEGMENT
Var1 DW 5
Var2 DW 8
Var3 DW 3
Dados ENDS
Codigo SEGMENT
ASSUME CS: Codigo
ASSUME DS: Dados
SomaMem MACRO Mem1, Mem2 ;; Soma duas variáveis
mov AX,Mem1
push AX
mov AX,Mem2
mov DX,AX
pop AX
add AX,DX
mov Mem1,AX
ENDM
Inicio:
mov AX, Dados
mov DS, AX
mov AX,2
mov AX, DX
mul DX
SomaMem Var1,V
mov AX, Var1
add AX, DX
mov Var2, AX
SomaMem Var1, 3
SomaMem Var1, Var3
CODIGO ENDS
END Inicio
`,[]);
ProgramToMemoryEventObserver = setInterval(() => asmr.observeProgramToMemoryFiring(getEmitter()), 10);
}

run();