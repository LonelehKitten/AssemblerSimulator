import { useEffect, useState } from 'react';
import {
  ThemeProvider,
  createTheme,
  makeStyles,
} from '@material-ui/core/styles';
import Backdrop from '@material-ui/core/Backdrop';
import CircularProgress from '@material-ui/core/CircularProgress';

import Header from './theme/header';
import Editor from './view/editor';
import Console from './view/console';
import Register from './view/register';

import Dragger from './utils/dragger';
import { Context } from './utils/context';
import Alert from './utils/alert';

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

const useStyles = makeStyles((theme) => ({
  main: {
    position: 'relative',
    float: 'left',
    backgroundColor: theme.palette.secondary.main,
    height: 'calc(100vh - 48px)',
  },
  sidebar: {
    position: 'relative',
    float: 'left',
    backgroundColor: '#21222c',
    height: 'calc(100vh - 48px)',
  },
}));

/*
const {ipcRenderer} = window.electron;
ipcRenderer.once(event, callback); // Para receber o evento Electron -> React
ipcRenderer.send(event, data?); // Para enviar o evento React -> Electron

    abc: { name: "Arquivo 1", code: "add" },
    abce: { name: "Arquivo 2", code: "add aX" }
  
*/
function App2() {
  const classes = useStyles();

  const [listFiles, setListFiles] = useState(() => {
    const list = JSON.parse(window.localStorage.getItem('_listFiles'));

    return list || {};
  });

  const [consoleFlag, setConsoleFlag] = useState(false);

  const [playing, setPlaying] = useState(false);
  const [playButtonPressed, setPlayButtonPressed] = useState('');
  const [currentID, setCurrentID] = useState('');
  const [currentFile, setCurrentFile] = useState(null);
  const [alertMessage, setAlertMessage] = useState(null);
  const [EtoC, setEtoC] = useState(250); // EtoC = Editor to Console
  const [EtoR, setEtoR] = useState(460); // EtoR = Editor to Register

  const handleVertical = (e) => {
    const newWidth = Math.abs(
      e.clientX - document.body.offsetLeft - document.body.offsetWidth
    );
    if (newWidth > 300 && newWidth < document.body.offsetWidth / 2) {
      setEtoR(newWidth);
    }
  };

  const handleHorizontal = (e) => {
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
      if (item.path === path) idExists = k;
    });
    if (idExists !== null) {
      setCurrentID(idExists);
      setCurrentFile(listFiles[idExists]);
      alertShow('info', 'Esse arquivo já está aberto.');
      return;
    }
    if (name == null) name = path.replace("/", "\\").split('\\').slice(-1)[0];
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

  useEffect(() => {
    window.localStorage.setItem('_listFiles', JSON.stringify(listFiles));
  }, [listFiles]);

  return (
    <Context.Provider
      value={{
        playing,
        setPlaying,
        currentFile,
        listFiles,
        currentID,
        setListFiles,
        addFile,
        setCode,
        changeFile,
        alertShow,
        playButtonPressed,
        setPlayButtonPressed,
        consoleFlag,
        setConsoleFlag,
      }}
    >
      <Alert onClose={setAlertMessage} message={alertMessage} />
      <Header />
      <main
        className={classes.main}
        style={{ width: `calc(100% - ${EtoR}px)` }}
      >
        <Editor style={{ height: `calc(100vh - 48px - ${EtoC}px)` }} />
        <Dragger orientation='horizontal' onMouse={handleHorizontal} />
        <Console
          style={{ height: `calc(${EtoC}px - 8px)`, marginLeft: '3px' }}
          width={EtoR}
        />
      </main>
      <aside className={classes.sidebar} style={{ width: EtoR + 'px' }}>
        <Dragger onMouse={handleVertical} />
        <Register
          style={{ minWidth: '15vw' }}
          width={EtoR}
          height={`calc(100vh)`}
        />
      </aside>
    </Context.Provider>
  );
}

const Loading = () => (
  <Backdrop style={{ color: '#fff' }} open={true}>
    {' '}
    <CircularProgress color='inherit' />
  </Backdrop>
);

const App = () => {
  const [loading, setLoading] = useState(true);

  useState(() => {
    setTimeout(() => {
      setLoading(false);
    }, 1000);
  }, []);

  return loading ? (
    <Loading />
  ) : (
    <ThemeProvider theme={theme}>
      <App2 />
    </ThemeProvider>
  );
};

export default App;
