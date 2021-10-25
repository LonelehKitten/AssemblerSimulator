import React, { useEffect } from 'react';
import { makeStyles } from '@material-ui/core/styles';
import Table from '@material-ui/core/Table';
import TableBody from '@material-ui/core/TableBody';
import TableCell from '@material-ui/core/TableCell';
import TableContainer from '@material-ui/core/TableContainer';
import TableHead from '@material-ui/core/TableHead';
import TableRow from '@material-ui/core/TableRow';
import Paper from '@material-ui/core/Paper';
import Brightness1Icon from '@material-ui/icons/Brightness1';

import { useContext } from '../../utils/context';
//];

const useStyles = makeStyles((theme) => ({
  root: {
    display: 'flex',
    flexWrap: 'wrap',
    marginTop: '0',
    height: '88%', //100% //'calc(70vh - 3rem)',
  },
  table: {
    alignContent: 'center',
    alignItems: 'center',
    justifyContent: 'center',
    color: 'inherit',
    backgroundColor: '#313241',
    overflow: 'auto',
    overflowY: 'scroll',
    '& .MuiTableCell-root': {
      borderColor: '#999',
      color: '#fff',
    },
  },
  register: {
    width: '100%',
    height: '5em',
    margin: '.25em 1em',
    backgroundColor: '#313241',
    borderRadius: '.75em',
    borderTop: '.2em solid #6e00ff',
    boxShadow: '0 .2em .5em #0005',
    display: 'flex',
    justifyContent: 'center',
    alignItems: 'center',
    fontWeight: 'bold',
    textAlign: 'center',
    '& .name': {
      display: 'flex',
      width: '25%',
      justifyContent: 'center',
      alignItems: 'center',
    },
    '& .bits': {
      display: 'flex',
      flexWrap: 'wrap',
      width: '50%',
      justifyContent: 'center',
      alignItems: 'center',
      '& > div': {
        width: '10.5%',
        height: '50%',
        margin: '1%',
        '&:nth-child(1)': {
          borderRadius: '.75em 0 0 0',
        },
        '&:nth-child(8)': {
          borderRadius: '0 .75em 0 0',
        },
        '&:nth-child(9)': {
          borderRadius: '0 0 0 .75em',
        },
        '&:nth-child(16)': {
          borderRadius: '0 0 .75em 0',
        },
      },
    },
    '& .value': {
      width: '25%',
      display: 'flex',
      justifyContent: 'center',
      alignItems: 'center',
    },
  },
}));
/*
const Bit = ({ value }) => (
  <Brightness1Icon
    style={{
      fontSize: 10,
      color: value == 1 ? '#1956c500' : '#1956c5',
      border: '1px solid #1956c5'
    }}
  />
);
*/
const Bit = ({ value }) => (
  <div
    style={{
      fontSize: 10,
      backgroundColor: value == 1 ? '#1b6cff' : '#1b6cff00',
      border: '1px solid #1b6cff',
      width: '.7em',
      height: '.7em',
      display: 'inline-block',
      borderRadius: '50%',
      margin: '0 .1em',
    }}
  ></div>
);

const Register = ({ name, value }) => {
  const classes = useStyles();
  const bits = parseInt(value).toString('2').padStart(16, 0);
  return (
    <div className={classes.register}>
      <div className='name'>{name}</div>
      <div className='bits'>
        {[...bits].map((bit, key) => (
          <div
            key={key}
            style={{ backgroundColor: bit === '1' ? '#1b6cff' : '#1b6cff44' }}
          >
            &nbsp;
          </div>
        ))}
      </div>
      <div className='value'>{parseInt(value).toString(16).padStart(4, 0)}</div>
    </div>
  );
};

const LowerMenu = ({ register }) => {
  const classes = useStyles();
  return (
    <section className={classes.root}>
      <Register name='AX' value={register.AX} />
      <Register name='DX' value={register.DX} />
      <Register name='SI' value={register.SI} />
      <Register name='DS' value={register.DS} />
      <Register name='CS' value={register.CS} />
      <Register name='SS' value={register.SS} />
      <Register name='SP' value={register.SP} />
    </section>
  );
};

/*

<TableContainer
        component={Paper}
        style={{ margin: '10px', backgroundColor: '#313241' }}
      >
        <Table className={classes.table} aria-label='resgistry operation table'>
          <TableHead>
            <TableRow>
              <TableCell align='center'>Registrador</TableCell>
              <TableCell align='center'>Bits</TableCell>
              <TableCell align='center'>Valor</TableCell>
            </TableRow>
          </TableHead>
          <TableBody>
            {Object.entries(register).map(([row, value], key) => {
              if (row == 'SR') return;
              const bits = parseInt(value).toString('2').padStart(16, 0);
              return (
                <TableRow key={key}>
                  <TableCell align='center'>{row}</TableCell>
                  <TableCell align='center'>
                    {[...bits].map((item) => (
                      <Bit value={item} />
                    ))}
                  </TableCell>
                  <TableCell align='center'>{parseInt(value)}</TableCell>
                </TableRow>
              );
            })}
          </TableBody>
        </Table>
      </TableContainer>

*/

export default LowerMenu;
