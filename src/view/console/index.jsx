import { useState } from 'react';

import { makeStyles } from '@material-ui/core/styles';

import List from '@mui/material/List';
import ListItem from '@mui/material/ListItem';
import ListItemText from '@mui/material/ListItemText';

const Console = (props) => {
  const useStyles = makeStyles((theme) => ({
    '@global': {
      '*::-webkit-scrollbar': {
        width: '0.4em',
        height: '0.4em',
      },
      '*::-webkit-scrollbar-track': {
        '-webkit-box-shadow': 'inset 0 0 6px rgba(0,0,0,0.02)',
      },
      '*::-webkit-scrollbar-thumb': {
        backgroundColor: '#191b24',
        outline: '1px solid ',
      },
    },
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
    },
    inputedTexts: {
      backgroundColor: '#44475a',
      borderRadius: '20px 0px 0px 20px',
      minHeight: '30px',
      marginTop: '5px',
    },
  }));

  const [history, setHistory] = useState([]);

  const classes = useStyles();

  const handleSubmit = (e) => {
    if (e.keyCode == 13) {
      const value = e.target.value;
      if (value == '') return;
      setHistory((oldValue) => [...oldValue, value]);
      e.target.value = '';
    }
  };

  return (
    <div id='console' {...props}>
      <List
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
      >
        {history.map((item) => (
          <ListItem className={classes.inputedTexts}>
            <ListItemText primary={`> ⠀ ${item}`} />
          </ListItem>
        ))}
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
