const { ipcRenderer } = window.electron

const event = (name, params, callback = null) => {
    ipcRenderer.send.apply(this, [name, ...params]);
    if(callback != null) ipcRenderer.once(name, callback);
}
export default event;