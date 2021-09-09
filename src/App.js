import { useState } from 'react';
import { ThemeProvider, createTheme, makeStyles } from '@material-ui/core/styles';

import Header from './theme/header';
import Editor from './view/editor';
import Console from './view/console';
import Register from './view/register';

import Dragger from './utils/dragger';

const theme = createTheme({
  palette: {
    /*
    primary: {
      main: '#1eb2a6',
      contrastText: '#fff',
    },
    secondary: {
      main: '#ffa34d',
      contrastText: '#fff',
    },*/
  },
});

const useStyles = makeStyles((theme) => ({
  main: {
    position: 'relative',
    float: 'left',
    backgroundColor: theme.palette.secondary.main,
    height: 'calc(100vh - 48px)'
  },
  sidebar: {
    position: 'relative',
    float: 'left',
    backgroundColor: theme.palette.secondary.main,
    height: 'calc(100vh - 48px)'
  }
}));

function App() {
  const classes = useStyles();

  const [EtoC, setEtoC] = useState(50); // EtoC = Editor to Console
  const [EtoR, setEtoR] = useState(460); // EtoR = Editor to Register

  const handleVertical = (e) => {
    const newWidth = Math.abs(e.clientX - document.body.offsetLeft - document.body.offsetWidth);
    if (newWidth > 300 && newWidth < document.body.offsetWidth / 2 || true) {
      setEtoR(newWidth);
    }
  };

  const handleHorizontal = (e) => {
    const newHeight = Math.abs(e.clientY - document.body.offsetHeight - document.body.offsetTop);
    if (newHeight > 300 && newHeight < document.body.offsetHeight / 2 || true) {
      setEtoC(newHeight);
    }
  };

  return (
    <ThemeProvider theme={theme}>
      <Header />
      <main className={classes.main} style={{ width: `calc(100% - ${EtoR}px)` }}>
        <Editor style={{ height: `calc(100vh - 48px - ${EtoC}px)` }} />
        <Dragger orientation="horizontal" onMouse={handleHorizontal} />
        <Console style={{ height: `calc(${EtoC}px - 48px)` }} />
      </main>
      <aside className={classes.sidebar} style={{ width: EtoR + "px" }}>
        <Dragger onMouse={handleVertical} />
        <Register />
      </aside>
    </ThemeProvider>
  );
}

export default App;
