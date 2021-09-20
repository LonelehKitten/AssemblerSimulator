const addon = require('bindings')('addon.node');
//const addon = require('../build/Release/addon.node');
const EventEmitter = require('events');
const fs = require('fs');
const { BrowserWindow, ipcMain, dialog, webContents } = require('electron');

// callback(function (event, data) {
//   webContents.getFocusedWebContents().send('on_event', event, data);
//   console.log(event, data);
// });]

ipcMain.on('play_expandMacros', (event, code) => {
    const emitter = new EventEmitter();
    console.log(code);
    emitter.on('success', (evt) => {
        event.sender.send('on_console', evt);
        event.sender.send('expand_macro', evt);
        console.log('### START ... ' + evt);
    });
    //event.sender.send('expand_macro', "novo codigo"); //  Para debugar
    addon.expandMacros(code, emitter.emit.bind(emitter));
});

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
