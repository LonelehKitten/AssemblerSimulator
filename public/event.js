const bindings = require('bindings')
//const addon = require('../build/Release/addon.node');
const EventEmitter = require('events');
const fs = require('fs');
const { BrowserWindow, ipcMain, dialog, webContents } = require('electron');

//const isAsmr = typeof asmr != "undefined"; // Para não dar pau no windows
// require('bindings')('ASMR')
let asmr;
try{
    asmr = bindings('ASMR') ?? null; // Pega o ASMR se ele estiver compilado
}catch(e){
    asmr = null;
}
console.log(asmr);
const isAsmr = asmr != null; 
if(isAsmr) asmr.init();
const emitter = new EventEmitter();
let playing = false;
let MacroExpandedEventObserver = null, CycleEventObserver = null, LogEventObserver = null;

const getCurrentBrowser = () => BrowserWindow.getAllWindows()[0];

// Eventos
emitter.on('macroExpanded', (data) => {
    getCurrentBrowser()?.webContents.send("macroExpanded",data);
    clearInterval(MacroExpandedEventObserver)
    playing = false;
});
emitter.on('cycle', (data) => {
    if(data == "halt"){
        playing = false;
        clearInterval(CycleEventObserver)
    }else{
        const array = JSON.parse(data);
        console.log(array);
        getCurrentBrowser()?.webContents.send("cycle_memory",array.memoryChanges);
        getCurrentBrowser()?.webContents.send("cycle_registers",array.registers);
        getCurrentBrowser()?.webContents.send("cycle_stdin",array.stdin);
        if(stdout != "") getCurrentBrowser()?.webContents.send("console",{message: array.stdout,type:0});
    }
});
emitter.on('log', (data) => {
    getCurrentBrowser()?.webContents.send("console",JSON.parse(data));
});

const getEmitter = () => emitter.emit.bind(emitter);

LogEventObserver = setInterval(() => asmr?.observeLogFiring(getEmitter()), 10);

// Requisições
const requests = [
    'requestExpandMacros',
    'requestTest',
    'requestAssembleAndRun',
    'requestAssembleAndRunBySteps',
    'requestRun',
    'requestRunBySteps',
    'requestNextStep',
    'requestKillProcess',
    'requestClockChange',
    'requestSendInput'
];
ipcMain.on("play", (event, type, params) => {
    // Executa as requisições após o evento do front
    if(requests.includes(type) && isAsmr){
        asmr[type].apply(asmr,params);
        playing = true;

        // Adicionar os observações
        switch(type){
            case "requestExpandMacros":
                MacroExpandedEventObserver = setInterval(() => asmr.observeExpandedMacrosFiring(getEmitter()), 10);
                break;
            case "requestAssembleAndRun":
            case "requestAssembleAndRunBySteps":
            case "requestRun":
            case "requestRunBySteps":
                CycleEventObserver = setInterval(() => asmr.observeCycleFiring(getEmitter()), 10);
                break;
        }
    }
    // Para simulações das memorias e registradores
    if(type == "simulate"){
        const array = JSON.parse(simulate());
        getCurrentBrowser()?.webContents.send("cycle_memory",array.memoryChanges);
        getCurrentBrowser()?.webContents.send("cycle_registers",array.registers);
        getCurrentBrowser()?.webContents.send("cycle_stdin",array.stdin);
        if(array.stdout != '') getCurrentBrowser()?.webContents.send("console",{message: array.stdout,type:0});
    }
})

// Simulação
const simulate = () => {
    const json = {
        registers: {
            AX: Math.random() * 50,
            DX: Math.random() * 50,
            SI: Math.random() * 50,
            SS: Math.random() * 50,
            DS: Math.random() * 50,
            CS: Math.random() * 50,
            SP: Math.random() * 50,
            PC: Math.random() * 50,
            SR: {
                asLiteral: Math.random() * 50,
                asFlags: [randomBool(),false,false,false,false,false,randomBool(),randomBool(),randomBool(),randomBool(),false,false,randomBool()]
            }
        },
        stdout: Math.random() % 5 == 0 ? "Teste "+Date.now() : "",
        stdin: Math.random() % 2 == 0,
        memoryChanges:{
            address: 10,
            newValue: Math.floor(Math.random() * 1000)
        }
    }
    return JSON.stringify(json);
}

const randomBool = (n = 2) => {
    return Math.floor(Math.random() * n);
}

// Salvar Arquivo
ipcMain.on('invoke_save_file', async (event, data) => {
    let file = JSON.parse(data);
    if (file.path == '') {
        const result = await dialog.showSaveDialog(
            BrowserWindow.getFocusedWindow(),
            {
                filters: [
                    { name: 'Assembly File', extensions: ['asm'] },
                    { name: 'All Files', extensions: ['*'] },
                ],
            }
        );
        if (result.canceled) return event.sender.send('save_file', false);
        file.path = result.filePath;
        console.log(file.path);
    }
    fs.writeFile(file.path, file.code, function (err) {
        if (err) {
            console.log(err);
            event.sender.send('save_file', false);
        } else {
            event.sender.send('save_file', true, file.path);
            console.log('JSON saved');
        }
    });
});

// Carregar Arquivo
ipcMain.on('invoke_open_file', async (event, data) => {
    const result = await dialog.showOpenDialog(BrowserWindow.getFocusedWindow(), {
        properties: ['openFile'],
        filters: [
            { name: 'Assembly File', extensions: ['asm'] },
            { name: 'All Files', extensions: ['*'] },
        ],
    });
    if (result.canceled) return event.sender.send('open_file', false);
    const path = result.filePaths[0];
    try {
        const data = fs.readFileSync(path, 'utf8');
        console.log(data);
        return event.sender.send('open_file', path, data);
    } catch (err) {
        console.error(err);
        event.sender.send('open_file', false, err);
    }
});

const ASMRFinish = () => {
    if(isAsmr) asmr.finish();
    clearInterval(LogEventObserver);
}

module.exports = {
    ASMRFinish
}