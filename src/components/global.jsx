import { useState, useEffect } from 'react';
import { Context } from '../utils/context';

const { ipcRenderer } = window.electron;


const Global = ({ value, children }) => {
    const [memory, setMemory] = useState(() => {
        const memoryChanges = [];
        for (let b = 0; b < 128; b++) {
            for (let i = 0; i < 32; i++) {
                for (let j = 0; j < 16; j++) {
                    memoryChanges.push(parseInt(0).toString('16').padStart(4, 0));
                }
            }
        }
        return memoryChanges;
    });

    const [listFiles, setListFiles] = useState(() => {
        const list = JSON.parse(window.localStorage.getItem('_listFiles'));

        return list || {};
    });

    const [stdin, setStdin] = useState(false);
    const [registers, setRegisters] = useState({
        AX: 0,
        DX: 0,
        SI: 0,
        SS: 0,
        DS: 0,
        CS: 0,
        SP: 0,
        PC: 0,
        SR: {
            asLiteral: 0,
            asFlags: [
                false,
                false,
                false,
                false,
                false,
                false,
                false,
                false,
                false,
                false,
                false,
                false,
                false,
            ],
        },
    });

    const [byStep, setByStep] = useState(false);
    const [playing, setPlaying] = useState(false); // Quando o programa está em execução
    const [currentFile, setCurrentFile] = useState(null);


    const changeFile = (id) => {
        if (listFiles[id] !== undefined) {
            setCurrentFile(listFiles[id]);
        } else if (id == null) {
            setCurrentFile(null);
        }
    };


    const addFile = (path, code, name = null) => {
        let idExists = null;
        Object.entries(listFiles).forEach(([k, item]) => {
            if (item.path === path && item.path != '') idExists = k;
        });
        if (idExists !== null) {
            setCurrentFile(listFiles[idExists]);
            // alertShow('info', 'Esse arquivo já está aberto.');
            return;
        }
        if (name == null) name = path.replace('/', '\\').split('\\').slice(-1)[0];
        const id = Math.random()
            .toString(36)
            .replace(/[^a-z]+/g, '')
            .substr(0, 5);
        const newValue = {
            ...listFiles,
            [id]: { id, name, path, code, originCode: code },
        };
        setCurrentFile(newValue[id]);
        setListFiles(newValue);
    };


    const setCode = (value, isOrigin = false) => {
        if (currentFile !== null) {
            setCurrentFile((file) => {
                file.code = value;
                if (isOrigin) file.originCode = value;
                file.isSave = file.code != file.originCode;
                return file;
            });
            setListFiles({ ...listFiles, ...{ [currentFile.id]: currentFile } });
        }
    };

    // Salvar
    useEffect(() => {
        window.localStorage.setItem('_listFiles', JSON.stringify(listFiles));
    }, [listFiles]);

    useEffect(() => {
        ipcRenderer.on('end', (evt) => {
            setPlaying(false);
        });
        ipcRenderer.on('cycle_memory', (evt, data) => {
            if (Object.keys(data).length === 0) return;
            setMemory((old) => {
                if (Object.keys(old).includes(data.address.toString())) {
                    old[data.address] = data.newValue.toString(16).padStart(4, 0);
                }
                return old;
            });
        });
        ipcRenderer.on('init_memory', (evt, data) => {
            setMemory(() => {
                const memoryChanges = [];
                for (let b = 0; b < data.length; b++) {
                    memoryChanges.push(data[b]);
                }
                return memoryChanges;
            });
        });
    }, []);

    const context = {
        playing,
        setPlaying,
        registers,
        setRegisters,
        currentFile,
        changeFile,
        listFiles,
        setListFiles,
        addFile,
        setCode,
        memory,
        setMemory,
        stdin,
        setStdin,
        byStep,
        setByStep,
    };

    return <Context.Provider value={{ ...context, ...value }}>{children}</Context.Provider>
}

export default Global;