import { useState, useRef, useEffect } from 'react';
import { makeStyles } from '@material-ui/core/styles';
import List from '@material-ui/core/List';
import ListItem from '@material-ui/core/ListItem';
import ListItemText from '@material-ui/core/ListItemText';

const useStyles = makeStyles((theme) => ({
  textField: {
    fontFamily: 'VT323',
    marginBottom: '5px',
    backgroundColor: 'transparent',
    border: `3px inset ${theme.palette.primary.main}`,
    padding: '10px',
    color: '#fff',
    backgroundColor: '#282a36',
    fontSize: 24,
    outline: 0,
    borderRadius: '3px',
    marginRight: '10px',
  },
  root: {
    overflow: 'auto',
    paddingRight: 10,
    '& .MuiTypography-root': {
      fontFamily: 'VT323 !important',
      lineBreak: 'anywhere',
      fontSize: 22,
    },
    '&::-webkit-scrollbar': {
      width: '0.3em',
      height: '0.4em',
    },
    '&::-webkit-scrollbar-track': {
      '-webkit-box-shadow': 'inset 0 0 6px rgba(0,0,0,0.5)',
    },
    '&::-webkit-scrollbar-thumb': {
      backgroundColor: '#191b24',
      outline: '0px solid ',
    },
    '&::-webkit-scrollbar-corner': {
      background: '#282a36',
    },
  },
  inputedTexts: {
    height: '30px',
    backgroundColor: '#44475a',
    borderRadius: '20px 0px 0px 20px',
    minHeight: '30px',
    marginTop: '5px',
  },
}));

const { ipcRenderer } = window.electron;

const Console = (props) => {
  const classes = useStyles();
  const consoleEndRef = useRef(null);
  const [history, setHistory] = useState([]);

  const handleSubmit = (e) => {
    if (e.keyCode == 13) {
      const value = e.target.value;
      if (value == '') return;
      setHistory((oldValue) => [...oldValue, value]);
      e.target.value = '';
      const element = consoleEndRef.current;
    }
  };

  useEffect(() => {
    consoleEndRef.current?.scrollIntoView({ behavior: 'instant' });
  }, [history]);

  useEffect(() => {
    ipcRenderer.on('on_console', (e, message) => {
      setHistory((oldValue) => [...oldValue, message]);
      console.log(message);
    });
  }, []);

  return (
    <div id='console' {...props}>
      <List className={classes.root}>
        {history.map((item, key) => (
          <ListItem key={key} className={classes.inputedTexts}>
            <ListItemText fontFamily='VT323' primary={`>⠀${item}`} />
          </ListItem>
        ))}
        <div ref={consoleEndRef}></div>
      </List>
      <input
        onKeyDown={handleSubmit}
        placeholder='Digite um comando . . .'
        className={classes.textField}
      />
    </div>
  );
};

export default Console;

/*
         sx={{
          position: 'relative',
          overflow: 'auto',
          '&::-webkit-scrollbar': {
            width: '0.3em',
            height: '0.4em',
          },
          '&::-webkit-scrollbar-track': {
            '-webkit-box-shadow': 'inset 0 0 6px rgba(0,0,0,0.5)',
          },
          '&::-webkit-scrollbar-thumb': {
            backgroundColor: '#191b24',
            outline: '0px solid ',
          },
        }}
        */
