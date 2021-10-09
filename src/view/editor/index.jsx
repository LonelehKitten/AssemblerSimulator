import React, { useState } from 'react';
import { useContext } from '../../utils/context';
import TabContainer from '@material-ui/core/Tabs';
import Tab from '@material-ui/core/Tab';import { makeStyles } from '@material-ui/core/styles';


import Ace from './ace';
import Tabs from './tabs';
import Memory from './memory';


const useStyles = makeStyles((theme) => ({
  root: {
    backgroundColor: '#282a36 ',
    color: '#fff',
    minHeight: '2rem',
    '& 	.MuiTab-root': {
      boxShadow: '0 3px 6px rgba(0,0,0,0.16), 0 3px 6px rgba(0,0,0,0.23)',
      padding: '0 12px',
      minHeight: '2rem',
    },
  },
  plus: {
    minWidth: 'auto',
  },
}));

const Editor = (props) => {
  const classes = useStyles();
  const { listFiles, changeFile } = useContext();
  const [menu,setMenu] = useState("editor");
  const [value, setValue] = useState('');

  const handleChange = (event, newValue) => {
    setValue(newValue);
    changeFile(newValue);
  };

  const changeMenu = (event,newValue) => {
    setMenu(newValue);
  }

  return (
    <div id='editor' {...props}>
      <TabContainer
        value={menu}
        onChange={changeMenu}
        indicatorColor='secondary'
        textColor='inherit'
        aria-label='disabled tabs example'
        variant='scrollable'
        scrollButtons='auto'
        className={classes.root}
      >
      <Tab
        value='editor'
        label='Editor'
      />
      <Tab
        value='memory'
        label='Memoria'
      />
      </TabContainer>
      {menu == "editor" && 
      <>
        <Tabs value={value} onChange={handleChange} listFiles={listFiles} />
        
        <Ace onChange={() => console.log('aa')} />
      </>}
      {menu == "memory" && <Memory />}
    </div>
  );
};
//<Memory />

export default Editor;
