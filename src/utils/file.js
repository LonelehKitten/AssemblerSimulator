const { ipcRenderer } = window.electron;

import event from './event';

const save = ({ alertShow, currentFile, setCode, setListFiles }) => {
    if (currentFile?.isSave) {
        ipcRenderer.send('invoke_save_file', JSON.stringify(currentFile));
        ipcRenderer.once('save_file', (e, success, path) => {
            if (success) {
                setCode(currentFile.code, true);
                if (currentFile.path == '') {
                    currentFile.path = path;
                    currentFile.name = path.split('\\').slice(-1)[0];
                    setListFiles((listFiles) => ({ ...listFiles, ...{ [currentFile.id]: currentFile } }));
                }
                alertShow('success', 'Arquivo Salvo');
            } else {
                alertShow('danger', 'Erro ao salvar o arquivo');
            }
        });
    }
}

const load = ({ alertShow, addFile }, path = "") => {
    ipcRenderer.send('invoke_open_file', path);
    ipcRenderer.once('open_file', (e, path, code = '') => {
        if (path === false) {
            if (code != '') {
                alertShow('error', code);
            }
        } else {
            alertShow('success', 'Arquivo aberto');
            console.log(path, code);
            addFile(path, code);
        }
    });
}

const close = ({ setListFiles, changeFile, currentFile }) => {
    if (currentFile == null) return;
    if (!currentFile.isSave || confirm('Você não salvou esse arquivo, deseja continua?')) {
        const id = currentFile.id;
        setListFiles((listFiles) => {
            const newListFiles = { ...listFiles };
            delete newListFiles[id];

            return newListFiles;
        });
        changeFile(null);
    }
}

const getTree = ({setTreeFiles},directory = "") => {
    console.log(directory == "");
    event("openTreeDir", [directory], (e, tree, path) => {
        localStorage.setItem("directory_root", path);
        setTreeFiles(tree);
    });
}

export { save, load, close,getTree };