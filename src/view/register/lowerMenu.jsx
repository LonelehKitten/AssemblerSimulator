import React from 'react';
import { makeStyles } from '@material-ui/core/styles';
import Table from '@material-ui/core/Table';
import TableBody from '@material-ui/core/TableBody';
import TableCell from '@material-ui/core/TableCell';
import TableContainer from '@material-ui/core/TableContainer';
import TableHead from '@material-ui/core/TableHead';
import TableRow from '@material-ui/core/TableRow';
import Paper from '@material-ui/core/Paper';
import Brightness1Icon from '@material-ui/icons/Brightness1';

function createData(register, bits, value = null) {
  if(value == null) value = parseInt(bits,2);
  return { register, bits, value };
}

const registers = [
  "AX","DX","SI","DS","SS","CS","SP","SR","PC"
]


const rows = registers.map((name) => createData(name,"0101010101101010"));
console.log("Rows",rows);
//];

const useStyles = makeStyles((theme) => ({
  root: {
    display: 'flex',
    marginTop: '0',
    padding: '10px',
    overflow: 'auto',
    height: 'calc(100% - 48px)'
    //height: `calc( ${props.height} - 16rem )`,
  },
  table: {
    alignContent: 'center',
    alignItems: 'center',
    justifyContent: 'center',
    color: 'inherit',
    backgroundColor: '#313241',
    overflow: 'auto',
    "& .MuiTableCell-root":{
      borderColor: '#999',
      color:"#fff"
    }
  },
}));

const Bit = ({value}) => <Brightness1Icon style={{fontSize:10,color:value == 0 ? "green": "red"}} />

const LowerMenu = (props) => {

  const classes = useStyles();

  return (
    <section className={classes.root}>
      <TableContainer component={Paper} style={{ backgroundColor: '#313241' }}>
        <Table className={classes.table} aria-label='resgistry operation table'>
          <TableHead>
            <TableRow>
              <TableCell align='center'>Registrador</TableCell>
              <TableCell align='center'>Bits</TableCell>
              <TableCell align='center'>Valor</TableCell>
            </TableRow>
          </TableHead>
          <TableBody>
            {rows.map((row,key) => (
              <TableRow key={key}>
                <TableCell align='center'>
                  {row.register}
                </TableCell>
                <TableCell align='center'>
                  {[...row.bits].map((item) => <Bit value={item} />)}
                </TableCell>
                <TableCell align='center'>
                  {row.value}
                </TableCell>
              </TableRow>
            ))}
          </TableBody>
        </Table>
      </TableContainer>
    </section>
  );
};

export default LowerMenu;
