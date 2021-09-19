const { app, BrowserWindow } = require('electron');
const isDev = require('electron-is-dev');
const path = require('path');

require('@electron/remote/main').initialize();
require('./event');

function createWindow() {
  // Create the browser window.
  const window = new BrowserWindow({
    webPreferences: {},
    width: 1200,
    height: 800,
    minHeight: 600,
    minWidth: 800,
    useContentSize: true,
    resizable: true,
    webPreferences: {
      contextIsolation: false,
      enableRemoteModule: true,
      nodeIntegration: true,
      nodeIntegrationInWorker: true,
      contextIsolation: false,
      preload: __dirname + '/preload.js',
    },
    icon: path.join(__dirname, 'AS.ico'),
  }); //'http://localhost:3000'
  window.removeMenu();
  window.loadURL(
    isDev
      ? 'http://localhost:3000'
      : `file://${path.join(__dirname, '../build/index.html')}`
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
});

app.on('activate', function () {
  // On OS X it's common to re-create a window in the app when the
  // dock icon is clicked and there are no other windows open.
  if (BrowserWindow.getAllWindows().length === 0) createWindow();
});
