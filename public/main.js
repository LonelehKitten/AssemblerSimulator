const {app, BrowserWindow} = require('electron');
const isDev = require('electron-is-dev');
const path = require('path');

require('@electron/remote/main').initialize();

function createWindow() {
    // Create the browser window.
    const window = new BrowserWindow({
        width: 800,
        height: 600,
        webPreferences: {
            nodeIntegration: true,
            enableRemoteModule: true,
        }
    });
    window.loadURL(
        isDev ? 'http://localhost:3000' : `file://${path.join(__dirname, '../build/index.html')}`
    );
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