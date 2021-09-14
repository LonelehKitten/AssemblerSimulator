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
    createData(237, 9.0, 37, 4.3),
    createData(262, 16.0, 24, 6.0),
    createData(305, 3.7, 67, 4.3),
    createData(356, 16.0, 49, 3.9),
];

const LowerMenu = (props) => {
    const useStyles = makeStyles((theme) => ({
        root: {
            color: theme.palette.primary.contrastText,
            display: "flex",
            margin: "1rem",
            padding: "10px",
        },
        table: {
            alignContent: "center",
            alignItems: "center",
            justifyContent: "center",
            color: "inherit",
            backgroundColor: theme.palette.primary.main,
        },
        override: {
            MuiTable: {
                body: {
                    color: theme.palette.primary.contrastText,
                },
            },
        },
    }));

    const classes = useStyles();

    return (
        <section className={classes.root}>
            <TableContainer component={Paper}>
                <Table className={classes.table} aria-label="resgistry operation table">
                    <TableHead className={classes.textColor}>
                        <TableRow>
                            <TableCell align="center">Origin</TableCell>
                            <TableCell align="center">OpCode</TableCell>
                            <TableCell align="center">Value</TableCell>
                            <TableCell align="center">Destiny</TableCell>
                        </TableRow>
                    </TableHead>
                    <TableBody className={classes.textColor}>
                    {rows.map((row) => (
                        <TableRow key={row.name}>
                        <TableCell align="center">{row.Origin}</TableCell>
                        <TableCell align="center">{row.OpCode}</TableCell>
                        <TableCell align="center">{row.Value}</TableCell>
                        <TableCell align="center">{row.Destiny}</TableCell>
                        </TableRow>
                    ))}
                    </TableBody>
                </Table>
            </TableContainer>
        </section>
    );
}

export default LowerMenu;