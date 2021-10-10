import { useState, useRef, useEffect } from 'react';
import { makeStyles } from '@material-ui/core/styles';
import List from '@material-ui/core/List';
import ListItem from '@material-ui/core/ListItem';
import ListItemText from '@material-ui/core/ListItemText';
import ListItemIcon from '@material-ui/core/ListItemIcon';

import { useContext } from '../../utils/context';

import InfoIcon from '@material-ui/icons/Info';
import CheckCircleIcon from '@material-ui/icons/CheckCircle';
import ErrorIcon from '@material-ui/icons/Error';

const messagesIcon = {
  0: <InfoIcon />,
  1: <ErrorIcon />,
  2: <CheckCircleIcon />
}

const messagesType = {
  0: 'info',
  1: 'error',
  2: 'success'
}

const useStyles = makeStyles((theme) => ({
  textField: {
    fontFamily: 'VT323',
    marginBottom: '5px',
    backgroundColor: 'transparent',
    border: `3px inset ${theme.palette.primary.main}`,
    padding: '10px',
    color: '#fff',
    backgroundColor: '#21222c',
    fontSize: 24,
    outline: 0,
    borderRadius: '3px'
  },
  root: {
    overflow: 'auto',
    paddingRight: 10,
    '& .MuiTypography-root': {
      fontFamily: 'VT323 !important',
      lineBreak: 'anywhere',
      fontSize: 22,
    },
    '& .MuiListItem-root': {
      paddingTop: 0,
      paddingBottom: 0,
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
    backgroundColor: '#343746',
    borderRadius: '20px 0px 0px 20px',
    minHeight: '30px',
    marginTop: '5px',
    "& .MuiListItemIcon-root":{
      minWidth: 28
    },
    "&.info": {
      color: "rgb(166, 213, 250)",
      "& .MuiSvgIcon-root": {
        color: "rgb(166, 213, 250)",
      }
    },
    "&.error": {
      color: "rgb(250, 179, 174)",
      "& .MuiSvgIcon-root": {
        color: "rgb(250, 179, 174)",
      }
    },
    "&.success": {
      color: "rgb(183, 223, 185)",
      "& .MuiSvgIcon-root": {
        color: "rgb(183, 223, 185)",
      }
    }
  },
}));

const { ipcRenderer } = window.electron;
/*
0 = Info
1 = Error
2 = Sucess
*/
const Console = (props) => {
  const {
    addFile,
    alertShow
  } = useContext();

  const classes = useStyles();
  const consoleEndRef = useRef(null);
  const [history, setHistory] = useState([]);
  const [stdIn,setstdIn] = useState(false);

  const handleSubmit = (e) => {
    if (e.keyCode == 13) {
      const message = e.target.value;
      if (message === '') return;
      const data = [{ message, type: 0 }];
      console.log(data);
      setHistory((oldValue) => [...oldValue, ...data]);
      e.target.value = '';
    }
  };

  useEffect(() => {
    consoleEndRef.current?.scrollIntoView({ behavior: 'instant' });
  }, [history]);

  /*
  // codigo pra teste : o resultado deve ser '3' //
name VALEUSEGMENT
add AX, DX
end VALEUSEGMENT
*/

  useEffect(() => {
    // Evento do Console
    ipcRenderer.on('console', (e, message) => {
      console.log("message",message);
      setHistory((oldValue) => [...oldValue, ...[message]]);
    });
    ipcRenderer.on('cycle_stdin',(value) => {
      setstdIn(value);
    });
  

    // Evento da MacroExpandida
    ipcRenderer.on('macroExpanded', (e, message) => {
      ipcRenderer.send(
        'invoke_save_file',
        JSON.stringify({
          code: message,
          path: ''
        })
      );
      ipcRenderer.once('save_file', (e, success, path) => {
        if (success) {
          addFile(path, message);
          alertShow('success', 'Arquivo Salvo');
        } else {
          alertShow('danger', 'Erro ao salvar o arquivo');
        }
      });
    });
  }, []);

  return (
    <div id='console' {...props}>
      <List className={classes.root}>
        {history.map((item, key) => (
          <ListItem key={key} className={classes.inputedTexts + " " + messagesType[item.type]}>
            <ListItemIcon>
              {messagesIcon[item.type]}
            </ListItemIcon>
            <ListItemText fontFamily='VT323' primary={item.message} />
          </ListItem>
        ))}
        <div ref={consoleEndRef}></div>
      </List>
      <input
        disabled={!stdIn}
        style={{display: stdIn ? 'inline-block' : 'none'}}
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
