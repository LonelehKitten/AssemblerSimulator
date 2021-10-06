const { ipcRenderer } = window.electron

const event = (name, params, callback) => {
    ipcRenderer.send.apply(this, [name, ...params]);
    ipcRenderer.once(name, callback);
}
export default event;