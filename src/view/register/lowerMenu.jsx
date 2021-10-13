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

function createData(register, bits, value = null) {
  if (value == null) value = parseInt(bits, 2);
  return { register, bits, value };
}

const registers = ['AX', 'DX', 'SI', 'DS', 'SS', 'CS', 'SP', 'SR', 'PC'];

const rows = registers.map((name) => createData(name, '0101010101101010'));
console.log('Rows', rows);
//];

const useStyles = makeStyles((theme) => ({
  root: {
    display: 'flex',
    marginTop: '0',
    height: 'calc(70vh - 3rem)',
    maxHeight: '31.5rem',
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

const LowerMenu = ({ register }) => {
  const classes = useStyles();
  return (
    <section className={classes.root}>
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
    </section>
  );
};

export default LowerMenu;
