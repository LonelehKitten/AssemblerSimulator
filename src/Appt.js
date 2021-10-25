import React, { useState } from 'react';
import { ThemeProvider, createTheme, makeStyles } from '@material-ui/core/styles';

import Header from './theme/header';
import Footer from './theme/footer';

import Editor from './view/editor';
import Console from './view/console';
import Register from './view/register';
import SideBar from './view/sidebar';

import { Context } from './utils/context';
import Container from './utils/container';

const useStyles = makeStyles((theme) => ({
    container: {
        display: 'flex',
        width: 'calc(100% - 50px)',
        height: 'calc(100vh - 48px - 1.5em)'
    }
}));

const theme = createTheme({
    palette: {
        primary: {
            main: '#282a36',
            contrastText: '#fff',
        },
        secondary: {
            main: '#21222c',
            contrastText: '#fff',
        },
    },
});

const App = () => {
    const classes = useStyles();

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

    const [consoleOpen, setConsoleOpen] = useState(true);

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
            asFlags: [false, false, false, false, false, false, false, false, false, false, false, false, false]
        },
    });

    const memoryRefs = React.useRef([React.createRef(), React.createRef()]);
    const [byStep, setByStep] = useState(false);
    const [playing, setPlaying] = useState(false);
    const [currentID, setCurrentID] = useState('');
    const [currentFile, setCurrentFile] = useState(null);
    const [alertMessage, setAlertMessage] = useState(null);
    const [EtoC, setEtoC] = useState(250); // EtoC = Editor to Console
    const [EtoR, setEtoR] = useState(460); // EtoR = Editor to Register

    const handleVertical = (e) => {
        e.preventDefault();
        const newWidth = Math.abs(
            e.clientX - document.body.offsetLeft - document.body.offsetWidth
        );
        if (newWidth > 300 && newWidth < document.body.offsetWidth / 2) {
            setEtoR(newWidth);
        }
    };

    const handleHorizontal = (e) => {
        e.preventDefault();
        const newHeight = Math.abs(
            e.clientY - document.body.offsetHeight - document.body.offsetTop
        );
        if (newHeight > 200 && newHeight < document.body.offsetHeight / 2) {
            setEtoC(newHeight);
        }
    };

    const changeFile = (id) => {
        if (listFiles[id] !== undefined) {
            setCurrentFile(listFiles[id]);
            setCurrentID(id);
        } else if (id == null) {
            setCurrentFile(null);
            setCurrentID('');
        }
    };

    const addFile = (path, code, name = null) => {
        /*
        const newValue = listFiles;
        newValue[id] = {name,path,code};*/
        let idExists = null;
        Object.entries(listFiles).forEach(([k, item]) => {
            if (item.path === path && item.path != '') idExists = k;
        });
        if (idExists !== null) {
            setCurrentID(idExists);
            setCurrentFile(listFiles[idExists]);
            alertShow('info', 'Esse arquivo já está aberto.');
            return;
        }
        if (name == null) name = path.replace('/', '\\').split('\\').slice(-1)[0];
        const id = Math.random()
            .toString(36)
            .replace(/[^a-z]+/g, '')
            .substr(0, 5);
        const newValue = {
            ...listFiles,
            [id]: { name, path, code, originCode: code },
        };
        console.log(newValue);
        setCurrentID(id);
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
            setListFiles({ ...listFiles, ...{ [currentID]: currentFile } });
        }
    };

    const alertShow = (color, text, time = 6000) => {
        setAlertMessage({ color, text, time });
    };

    const context = {
        playing,
        setPlaying,
        registers,
        setRegisters,
        currentFile,
        changeFile,
        currentID,
        listFiles,
        setListFiles,
        addFile,
        setCode,
        alertShow,
        memory,
        setMemory,
        consoleOpen,
        setConsoleOpen,
        EtoC,
        stdin,
        setStdin,
        memoryRefs,
        byStep,
        setByStep
    };
    return (
        <ThemeProvider theme={theme}>

            <Context.Provider value={context}>
                <Header />
                <div style={{display: "flex"}}>
                    <SideBar />
                    <div className={classes.container}>
                        <Container value={EtoR}>
                            <Container orientation='horizontal' value={consoleOpen ? EtoC : 0}>
                                <Editor
                                    style={{
                                        width: '100%',
                                        height: `100%`,
                                    }}
                                />
                            </Container>
                            <Container
                                draggable
                                orientation='horizontal'
                                onResize={handleHorizontal}
                                value={consoleOpen ? EtoC : 0}
                            >
                                <Console
                                    style={{
                                        width: '100%',
                                        height: `100%`,
                                    }}
                                />
                            </Container>
                        </Container>
                        <Container
                            draggable
                            onResize={handleVertical}
                            value={EtoR}
                            style={{ backgroundColor: '#21222c' }}
                        >
                            <Register
                                style={{ minWidth: '15vw' }}
                                width={'100%'}
                                height={`calc(100vh)`}
                            />
                        </Container>
                    </div>
                </div>
                <Footer />
            </Context.Provider>
        </ThemeProvider>

    );
}

export default App;