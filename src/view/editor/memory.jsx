import { useEffect, useMemo, useState } from 'react';
import { makeStyles } from '@material-ui/core/styles';
import Table from '@material-ui/core/Table';
import TableBody from '@material-ui/core/TableBody';
import TableCell from '@material-ui/core/TableCell';
import TableContainer from '@material-ui/core/TableContainer';
import TableHead from '@material-ui/core/TableHead';
import TableRow from '@material-ui/core/TableRow';
import Paper from '@material-ui/core/Paper';



const rows = [{}, {}, {}, {}, {}];

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
        "& .MuiTableCell-root": {
            borderColor: '#999',
            color: "#fff"
        }
    },
    form: {
        display:"block"
    }
}));

const { ipcRenderer } = window.electron;

const Memory = () => {
    const classes = useStyles();

    const [memory, setMemory] = useState([]);
    const [page, setPage] = useState(1);

    useEffect(() => {
        const memoryChanges = [];
        let a = 0;
        for(let b=0; b< 128; b++){
            for (let i = 0; i < 32; i++) {
                for (let j = 0; j < 16; j++) {
                    memoryChanges.push(Math.floor(Math.random() * 100));
                }
            }
        }
        setMemory(memoryChanges);
        ipcRenderer.on("cycle_memory", (evt, data) => {
            setMemory(data);
        });
    }, []);

    const rows = useMemo(() => {
        const row = [];
        for(let i=0; i<32; i++) row.push([]);
        memory.slice(512 * (page-1),512 * page).map((item,key) => {
            const id = Math.floor(key / 16);
            if(row[id] == undefined) row[id] = [];
            row[id].push(<TableCell align='center'>{item.toString("16")}</TableCell>);
        });
        return row;
    }, [memory,page]);

    const handleKeyDown = (e) => {
        if(e.keyCode == 13) setPage(e.target.value);
    }
    return (
        <div className={classes.root}>
            <div className={classes.form}>
                <input type='number' defaultValue={page} onKeyDown={handleKeyDown}/>
            </div>
            <TableContainer component={Paper} style={{ backgroundColor: '#313241' }}>
                <Table className={classes.table} aria-label='resgistry operation table'>
                    <TableBody>
                        {rows.map((row, key) => (
                            <TableRow key={key}>
                                <TableCell align='center'>
                                    {String(parseInt(key * 16 + (page-1)*512)).padStart(5, 0)}
                                </TableCell>
                                {row}
                            </TableRow>
                        ))}
                    </TableBody>
                </Table>
            </TableContainer>
        </div>
    );
}

export default Memory;