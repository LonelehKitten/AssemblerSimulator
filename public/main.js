const fs = require("fs");
const { app, BrowserWindow, ipcMain, dialog } = require('electron');
const isDev = require('electron-is-dev');
const path = require('path');

require('@electron/remote/main').initialize();

function createWindow() {
    // Create the browser window.
    const window = new BrowserWindow({
        width: '100%',
        height: '100%',
        minHeight: 600,
        minWidth: 800,
        useContentSize: true,
        resizable: true,
        webPreferences: {
            preload: __dirname + '/preload.js',
            enableRemoteModule: true,
            nodeIntegration: true,
            contextIsolation: false,
        },
        icon: path.join(__dirname, 'AS.ico')
    });//'http://localhost:3000'
    window.removeMenu();
    window.loadURL(
        isDev ? 'http://localhost:3000' : `file://${path.join(__dirname, '../build/index.html')}`
    );
    //window.loadFile('public/dev.html');
    if (isDev) window.webContents.openDevTools();
}

app.on('ready', createWindow);

app.on('window-all-closed', function () {
    // On OS X it is common for application and their menu bar
    // to stay active until the user quits explicitly with Cmd + Q
    if (process.platform !== 'darwin') {
        app.quit();
    }
})

app.on('activate', function () {
    // On OS X it's common to re-create a window in the app when the
    // dock icon is clicked and there are no other windows open.
    if (BrowserWindow.getAllWindows().length === 0) createWindow();
})
//BrowserWindow.getFocusedWindow()
ipcMain.on('invoke_save_file', async (event, data) => {
    console.log(data);
    const result = await dialog.showSaveDialog(BrowserWindow.getFocusedWindow(), {
        filters: [
            { name: 'Assembly File', extensions: ['asm'] },
            { name: 'All Files', extensions: ['*'] }
        ]
    });
    fs.writeFile(data.path, data.code, function(err) {
        if (err) {
            console.log(err);
            event.sender.send('save_file', false);
        } else {
            event.sender.send('save_file', true);
            console.log("JSON saved");
        }
    })
})

ipcMain.on('invoke_open_file', async (event, data) => {
    const result = await dialog.showOpenDialog(BrowserWindow.getFocusedWindow(), {
        properties: ['openFile'],
        filters: [
            { name: 'Assembly File', extensions: ['asm'] },
            { name: 'All Files', extensions: ['*'] }
        ]
    });
    const path = result.filePaths[0];
    try {
        const data = fs.readFileSync(path, 'utf8')
        console.log(data)
        event.sender.send('open_file', path, data);
    } catch (err) {
        console.error(err)
    }
    /*
    fs.open(path, 'r', function (err, code) {
        console.log(err, code);
        event.sender.send('open_file', path, code);
    });*/
})