import React from 'react';
import { makeStyles } from '@material-ui/core/styles';
import Table from '@material-ui/core/Table';
import TableBody from '@material-ui/core/TableBody';
import TableCell from '@material-ui/core/TableCell';
import TableContainer from '@material-ui/core/TableContainer';
import TableHead from '@material-ui/core/TableHead';
import TableRow from '@material-ui/core/TableRow';
import Paper from '@material-ui/core/Paper';
  
function createData(Origin, OpCode, Value, Destiny ) {
    return { Origin, OpCode, Value, Destiny };
}

const rows = [
    createData(159, 6.0, 24, 4.0),
    createData(159, 6.0, 24, 4.0),
    createData(159, 6.0, 24, 4.0),
    createData(159, 6.0, 24, 4.0),
    createData(159, 6.0, 24, 4.0),
    createData(159, 6.0, 24, 4.0),
    createData(159, 6.0, 24, 4.0),
    createData(159, 6.0, 24, 4.0),    
];

const LowerMenu = (props) => {
    const useStyles = makeStyles((theme) => ({
        '@global': {
            '*::-webkit-scrollbar': {
                width: '0.4em',
                height: '0.4em',
            },
            '*::-webkit-scrollbar-track': {
                '-webkit-box-shadow': 'inset 0 0 6px rgba(0,0,0,0.02)'
            },
            '*::-webkit-scrollbar-thumb': {
                backgroundColor: '#191b24',
                outline: '1px solid '
            }
        },
        root: {
            display: "flex",
            margin: "1rem",
            marginTop: "0",
            padding: "10px",
            overflow: "auto",
            height: `calc( ${props.height} - 16rem )`
        },
        table: {
            alignContent: "center",
            alignItems: "center",
            justifyContent: "center",
            color: "inherit",
            backgroundColor: theme.palette.primary.main,
            overflow: "auto",
        },
    }));

    const classes = useStyles();
 
    return (
        <section className={classes.root}>
            <TableContainer component={Paper} style={{backgroundColor: "#282a36"}}>
                <Table className={classes.table} aria-label="resgistry operation table">
                    <TableHead >
                        <TableRow>
                            <TableCell align="center" style={{color: "#fff"}}>Origin</TableCell>
                            <TableCell align="center" style={{color: "#fff"}}>OpCode</TableCell>
                            <TableCell align="center" style={{color: "#fff"}}>Value</TableCell>
                            <TableCell align="center" style={{color: "#fff"}}>Destiny</TableCell>
                        </TableRow>
                    </TableHead>
                    <TableBody >
                    {rows.map((row) => (
                        <TableRow key={row.name}>
                        <TableCell align="center" style={{color: "#fff"}}>{row.Origin}</TableCell>
                        <TableCell align="center" style={{color: "#fff"}}>{row.OpCode}</TableCell>
                        <TableCell align="center" style={{color: "#fff"}}>{row.Value}</TableCell>
                        <TableCell align="center" style={{color: "#fff"}}>{row.Destiny}</TableCell>
                        </TableRow>
                    ))}
                    </TableBody>
                </Table>
            </TableContainer>
        </section>
    );
}

export default LowerMenu;