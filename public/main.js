const {app, BrowserWindow, ipcMain} = require('electron');
const isDev = require('electron-is-dev');
const path = require('path');

require('@electron/remote/main').initialize();

function createWindow() {
    // Create the browser window.
    const window = new BrowserWindow({
        width:'100%',
        height: '100%',
        useContentSize: true,
        resizable:true,
        webPreferences: {
            preload: __dirname + '/preload.js',
            enableRemoteModule: true,
            nodeIntegration: true,
            contextIsolation: false,
        },
        icon: path.join(__dirname,'AS.ico')
    });//'http://localhost:3000'
    window.removeMenu();
    window.loadURL(
        isDev ? 'http://localhost:3000' : `file://${path.join(__dirname, '../build/index.html')}`
    );
    //window.loadFile('public/dev.html');
    if(isDev) window.webContents.openDevTools();
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

ipcMain.on('invokeAction', (event, data) => {
    console.log("Chamou");
    var result = "test result!";
    event.sender.send('actionReply', result);
})