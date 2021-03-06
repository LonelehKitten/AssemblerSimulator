import { useEffect, useMemo, useState } from 'react';
import { makeStyles } from '@material-ui/core/styles';
import Table from '@material-ui/core/Table';
import TableBody from '@material-ui/core/TableBody';
import TableCell from '@material-ui/core/TableCell';
import TableContainer from '@material-ui/core/TableContainer';
import TableHead from '@material-ui/core/TableHead';
import TableRow from '@material-ui/core/TableRow';
import Paper from '@material-ui/core/Paper';
import { IconButton } from '@material-ui/core';

import ChevronLeftIcon from '@material-ui/icons/ChevronLeft';
import ChevronRightIcon from '@material-ui/icons/ChevronRight';
import { useContext } from '../../utils/context';

const rows = [{}, {}, {}, {}, {}];

const useStyles = makeStyles((theme) => ({
  root: {
    display: 'block',
    marginTop: '0',
    padding: '10px',
    overflow: 'auto',
    height: 'calc(100% - 48px)',
    //height: `calc( ${props.height} - 16rem )`,
  },
  table: {
    alignContent: 'center',
    alignItems: 'center',
    justifyContent: 'center',
    color: 'inherit',
    backgroundColor: '#313241',
    overflow: 'auto',
    '& .MuiTableCell-root': {
      borderColor: '#999',
      color: '#fff',
      maxWidth: '50px!important'
    },
  },
  form: {
    display: 'flex',
    flex: 1,
    '& input': {
      background: 'transparent',
      border: 'none',
      color: '#f1f1f1',
      outline: 'none !important',
      textAlign: 'center',
      marginTop: 12,
      width: 38,
      fontSize: 15,
      textAlign: 'right',
      "&::-webkit-inner-spin-button":{
        display: "none !important"
      }
    },
    "& .total":{
      fontSize: 15
    },
    '& .MuiIconButton-root': {
      color: '#fff',
      "&.Mui-disabled":{
        color: "rgba(255, 255, 255, 0.26)"
      }
    },
    '& .buttonRight': {
      textAlign: 'right',
    },
    '& .center': {
      textAlign: 'center',
    },
    '&>div': {
      flexGrow: 1,
    },
  },
  input: {
    backgroundColor: 'transparent',
    border: 'none',
    outline: 'none !important',
    color: '#f1f1f1',
  },
}));

const { ipcRenderer } = window.electron;

const Memory = () => {
  const classes = useStyles();
  const { memory, setMemory, memoryRefs } = useContext();
  const [page, setPage] = useState(1);

  //   const [rerender, setRerender] = useState(0);

  //   useEffect(async () => {
  //     setRerender(rerender + 1);
  //   }, [memory]);

  const handleMemoryChange = (e) => {
    if ('0123456789abcdef'.split('').includes(e.nativeEvent.data)) {
      const { name, value } = e.target;
      changeMemory(name, value.substr(0, 4));
    }
  };

  const handleBlur = (e) => {
    const { name, value } = e.target;
    changeMemory(name, value.substr(0, 4).padStart(4, 0));
  };

  const changeMemory = (address, value) => {
    const old = [...memory];
    old[address] = value;
    setMemory(old);
    /*setMemory((old) => {
      if (old[address] !== undefined) {
        old[address] = value;
      }
      return old;
    });*/
  };

  const rows = useMemo(() => {
    const row = [];
    for (let i = 0; i < 32; i++) row.push([]);
    memory.slice(512 * (page - 1), 512 * page).map((item, key) => {
      const id = Math.floor(key / 16);
      if (row[id] == undefined) row[id] = [];
      row[id].push(
        <TableCell align='center'>
          <input
            //ref={memoryRefs.current[key]}
            key={key}
            onBlur={handleBlur}
            onChange={handleMemoryChange}
            name={key}
            className={classes.input}
            value={item}
          />
        </TableCell>
      );
    });
    return row;
  }, [memory, page]);

  const handleKeyDown = (e) => {
    if (e.target.value == '') {
      setPage('');
    } else {
      handlePageChange(parseInt(e.target.value))();
    }
  };

  const handlePageChange = (current) => () => {
    if (current < 1) current = 1;
    if (current > 128) current = 128;
    setPage(current);
  };

  return (
    <div className={classes.root}>
      <div className={classes.form}>
        <div>
          <IconButton onClick={handlePageChange(page - 1)} disabled={page == 1}>
            <ChevronLeftIcon />
          </IconButton>
        </div>
        <div className='center'>
          <input type='number' value={page} onChange={handleKeyDown} /><span className='total'>/128</span>
        </div>
        <div className='buttonRight'>
          <IconButton onClick={handlePageChange(page + 1)} disabled={page == 128}>
            <ChevronRightIcon />
          </IconButton>
        </div>
      </div>
      <TableContainer
        component={Paper}
        style={{ backgroundColor: '#313241', height: 'calc(100% - 50px)' }}
      >
        <Table className={classes.table} aria-label='resgistry operation table'>
          <TableBody>
            {rows.map((row, key) => (
              <TableRow key={key}>
                <TableCell align='center'>
                  {parseInt(key * 16 + (page - 1) * 512)
                    .toString('16')
                    .padStart(5, 0)}
                </TableCell>
                {row}
              </TableRow>
            ))}
          </TableBody>
        </Table>
      </TableContainer>
    </div>
  );
};
export default Memory;
