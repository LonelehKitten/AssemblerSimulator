import { useState, useEffect } from 'react';
import { makeStyles } from '@material-ui/core/styles';
import UpperMenu from './upperMenu';
import LowerMenu from './lowerMenu';
import { ClassNames } from '@emotion/react';
//import Grid from '@material-ui/core/Grid';

import { useContext } from '../../utils/context';

// Esse não tem, mas ussa esse aqui: https://material-ui.com/pt/components/tables/

const useStyles = makeStyles((theme) => ({
  root: {
    backgroundColor: '#21222c',
    width: '100%',
    height: '100%',
  },
}));
/*

<UpperMenu width={props.width}></UpperMenu>
*/
const { ipcRenderer } = window.electron;

const Register = (props) => {
  const { registers, setRegisters } = useContext();

  const classes = useStyles();

  // const [register, setRegister] = useState({
  //   AX: Math.random() * 50,
  //   DX: Math.random() * 50,
  //   SI: Math.random() * 50,
  //   SS: Math.random() * 50,
  //   DS: Math.random() * 50,
  //   CS: Math.random() * 50,
  //   SP: Math.random() * 50,
  //   PC: Math.random() * 50,
  //   SR: {
  //     asLiteral: Math.random() * 50,
  //     asFlags: [true, false, true, true, false, true, true, false],
  //   },
  // });

  useEffect(() => {
    ipcRenderer.on('cycle_registers', (e, data) => {
      setRegisters(data);
    });
  }, []);

  return (
    <div id='register' className={classes.root}>
      <UpperMenu register={registers} />
      <LowerMenu register={registers} />
    </div>
  );
};

export default Register;
