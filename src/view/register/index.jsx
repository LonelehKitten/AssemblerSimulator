import { useState, useEffect } from 'react';
import { makeStyles } from '@material-ui/core/styles';
import UpperMenu from './upperMenu';
import LowerMenu from './lowerMenu';
import TabContainer from '@material-ui/core/Tabs';
import Tab from '@material-ui/core/Tab';
import { ClassNames } from '@emotion/react';
//import Grid from '@material-ui/core/Grid';

import { useContext } from '../../utils/context';

import Tree from '../tree';
// Esse não tem, mas ussa esse aqui: https://material-ui.com/pt/components/tables/

const useStyles = makeStyles((theme) => ({
  root: {
    backgroundColor: '#21222c',
    width: '100%',
    height: 'calc(100% - 1em)',
    overflowY: 'scroll',
    //borderRadius: '1em',
    userSelect: 'none'
  },
  tab: {
    backgroundColor: '#21222c',
    color: '#fff',
    minHeight: '2rem',
    '& 	.MuiTab-root': {
      // boxShadow: '0 3px 6px rgba(0,0,0,0.16), 0 3px 6px rgba(0,0,0,0.23)',
      padding: '0 12px',
      minHeight: '2rem',
    },
    '& .MuiTab-textColorInherit.Mui-selected': {
      backgroundColor: '#282a36 !important',
      borderTop: '1px solid hotpink !important',
    },
  }
}));
/*

<UpperMenu width={props.width}></UpperMenu>
*/
const { ipcRenderer } = window.electron;

const Register = (props) => {
  const classes = useStyles();
  const { registers, setRegisters } = useContext();
  const [menu, setMenu] = useState('registers');


  useEffect(() => {
      ipcRenderer.on('cycle_registers', (e, data) => {
         // console.log(data);
        setRegisters(data);
    });
  }, []);

  return (
    <div id='register' className={classes.root}>
      <TabContainer
        value={menu}
        onChange={(event, newValue) => setMenu(newValue)}
        indicatorColor='secondary'
        textColor='inherit'
        aria-label='disabled tabs example'
        variant='scrollable'
        scrollButtons='auto'
        className={classes.tab}
      >
        <Tab value='registers' label='Registradores' />
        <Tab value='tree' label='Arquivos' />
      </TabContainer>
      {menu == "registers" && <>
        <UpperMenu register={registers} />
        <LowerMenu register={registers} />
      </>
      || <Tree />}
    </div>
  );
};

export default Register;
