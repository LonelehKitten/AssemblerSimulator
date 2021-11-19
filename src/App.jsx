import React, { useState } from 'react';
import {
  ThemeProvider,
  createTheme,
  makeStyles,
} from '@material-ui/core/styles';
import Alert from '@material-ui/lab/Alert';
import Snackbar from '@material-ui/core/Snackbar';
import Header from './theme/header';
import Footer from './theme/footer';

import Editor from './view/editor';
import Console from './view/console';
import Register from './view/register';
import SideBar from './view/sidebar';
import Memory from './view/memory';

import { Context } from './utils/context';
import Container from './utils/container';
import Global from './components/global';
import Tabs from './components/tabs';

const useStyles = makeStyles((theme) => ({
  container: {
    display: 'flex',
    width: 'calc(100% - 50px)',
    //height: 'calc(100vh - 36px - 1.5em)',
  },
  main: {
    display: 'flex',
    height: 'calc(100% - 36px - 1.5em)',
  },
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

  const [EtoC, setEtoC] = useState(250); // EtoC = Editor to Console
  const [EtoR, setEtoR] = useState(460); // EtoR = Editor to Register
  const [alertMessage, setAlertMessage] = useState(null);
  const [consoleOpen, setConsoleOpen] = useState(true);

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

  const alertShow = (color, text, time = 6000) => {
    setAlertMessage({ color, text, time });
  };

  return (
    <ThemeProvider theme={theme}>
      {alertMessage != null &&
        <Snackbar open={true} autoHideDuration={alertMessage.time} onClose={() => setAlertMessage(null)}>
          <Alert severity={alertMessage.color}>{alertMessage.text}</Alert>
        </Snackbar>}
      <Global value={{ alertShow, consoleOpen, setConsoleOpen }}>
        <Header />
        <div className={classes.main}>
          <SideBar />
          <div className={classes.container}>
            <Container value={EtoR}>
              <Container
                orientation='horizontal'
                value={consoleOpen ? EtoC : 0}
              >
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
      </Global>
    </ThemeProvider>
  );
};

export default App;
