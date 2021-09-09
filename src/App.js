import { useState } from 'react';
import { ThemeProvider, createTheme, makeStyles } from '@material-ui/core/styles';

import Header from './theme/header';
import Editor from './view/editor';
import Console from './view/console';
import Register from './view/register';

import Dragger from './utils/dragger';

const theme = createTheme({
  palette: {
    
    primary: {
      main: '#282a36',
      contrastText: '#fff',
    },
    secondary: {
      main: '#44475a',
      contrastText: '#fff',
    },
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

function App2() {
  const classes = useStyles();

  const [EtoC, setEtoC] = useState(250); // EtoC = Editor to Console
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
    <>
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
    </>
  );
}

const App = () => (
<ThemeProvider theme={theme}><App2 /></ThemeProvider>
);

export default App;
