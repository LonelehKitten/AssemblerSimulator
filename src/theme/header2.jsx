import React, { useState, useEffect } from 'react';
import { makeStyles } from '@material-ui/core/styles';
import AppBar from '@material-ui/core/AppBar';
import Toolbar from '@material-ui/core/Toolbar';
import Typography from '@material-ui/core/Typography';
import Button from '@material-ui/core/Button';
import IconButton from '@material-ui/core/IconButton';
import Drawer from '@material-ui/core/Drawer';
import TreeView from '@material-ui/lab/TreeView';
import TreeItem from '@material-ui/lab/TreeItem';
import Tooltip from '@material-ui/core/Tooltip';
import Slider from '@material-ui/core/Slider';
import MenuItem from '@material-ui/core/MenuItem';
import Menu from '@material-ui/core/Menu';

import PlayArrowIcon from '@material-ui/icons/PlayArrow';
import StopIcon from '@material-ui/icons/Stop';
import SkipNextIcon from '@material-ui/icons/SkipNext';
import MenuIcon from '@material-ui/icons/Menu';
import ExpandMoreIcon from '@material-ui/icons/ExpandMore';
import ChevronRightIcon from '@material-ui/icons/ChevronRight';
import AllOutIcon from '@material-ui/icons/AllOut';
import BugReportIcon from '@material-ui/icons/BugReport';
import UpdateIcon from '@material-ui/icons/Update';
import DescriptionIcon from '@material-ui/icons/Description';
import FolderOpenIcon from '@material-ui/icons/FolderOpen';
import InfoIcon from '@material-ui/icons/Info';

import { useContext } from '../utils/context';
import event from '../utils/event';
import { isEmpty } from '../utils';
const { ipcRenderer } = window.electron;

const useStyles = makeStyles((theme) => ({
  root: {
    flexGrow: 1,
    backgroundColor: '#191a21',
  },
  menuButton: {
    marginRight: theme.spacing(2),
  },
  title: {
    flexGrow: 1,
  },
  button: {
    '&.Mui-disabled': {
      color: 'rgba(255,255,255,0.4)',
    },
  },
  slider: {
    '& .MuiSlider-valueLabel': {
      top: 'inherit',
      bottom: -51,
      '&>span': {
        transform: 'rotate(135deg)',
        '& >span': {
          transform: 'rotate(-135deg)',
          color: '#333',
        },
      },
    },
  },
  menu: {
    '& .MuiPaper-root': {
      backgroundColor: '#63668a !important',
      width: '15em',
    },
    '& ul': {
      listStyleType: 'none',
      padding: 0,
    },
  },
  menuOption: {
    fontSize: '.8em',
    fontWeight: 'bold',
    backgroundColor: '#fff0',
    color: 'white',
    padding: '1em',
    display: 'flex',
    justifyContent: 'flex-start',
    alignItems: 'center',
    cursor: 'pointer',
    transition: 'all .1s',
    '&:hover': {
      backgroundColor: '#fff8',
      color: 'black',
    },
  },
}));

const requests = [
  'requestExpandMacros',
  'requestTest',
  'requestAssembleAndRun',
  'requestAssembleAndRunBySteps',
  'requestRun',
  'requestRunBySteps',
  'requestNextStep',
  'requestKillProcess',
  'requestClockChange',
  'requestSendInput',
  'simulate',
];

const Header = () => {
  const { memory, currentFile, addFile, alertShow, playing, setPlaying,byStep,setByStep } = useContext();
  //const { addFile, currentID, alertShow } = useContext();
  const [anchorEl, setAnchorEl] = useState(null);
  const [clock, setClock] = useState(16);
  const classes = useStyles();

  const [open, setOpen] = useState(false);

  const handleDrawerOpen = () => {
    setOpen(true);
  };

  const handleDrawerClose = () => {
    setOpen(false);
  };

  const getBytecodeFromEditor = () => {
    return currentFile?.code
      .replace(/[^0-9a-fA-F]/g, '')
      .match(/.{2}/g)
      .map((current) => parseInt('0x'.concat(current)))
      .filter((current) => !isNaN(current));
  };

  const handlePlay = (type) => () => {
    let memoryToBytes = [];
    for (let i = 0; i < memory.length; i++) {
      let string = String(memory[i]).padStart(4, 0);
      memoryToBytes.push(parseInt(string.substr(0, 2), 16));
      memoryToBytes.push(parseInt(string.substr(2, 2), 16));
    }
    console.log(memoryToBytes);
    // play_expandMacros
    if (!isEmpty(currentFile?.code) || type == 'simulate') {
      // Habilita ou n??o o "Playing"
      if (type == 'simulate' || type == 'requestEndTest' || type == 'requestTest' || type == 'requestKillProcess') setPlaying(false);
      else setPlaying(true);

      const params = [];

      // Adicionar as instru????es
      if (['requestRun', 'requestRunBySteps'].includes(type)) {
        let arr = getBytecodeFromEditor()
        console.log(arr)
        params.push(arr);
      }
      if (['requestExpandMacros', 'requestAssembleAndRun', 'requestAssembleAndRunBySteps'].includes(type)) {
        params.push(currentFile?.code);
      }
      if (['requestAssembleAndRun', 'requestAssembleAndRunBySteps', 'requestRun', 'requestRunBySteps'].includes(type)) {
        params.push(memoryToBytes);
      }
      if(['requestRunBySteps','requestAssembleAndRunBySteps','requestNextStep'].includes(type)){
        console.log("Header|BySteyp",type,true);
        setByStep(true);
      }else{
        console.log("Header|BySteyp",type,false);
        setByStep(false);
      }
      event('play', [type, params]);
    }
  };

  const handleClickDebug = (type) => () => {
    handlePlay(type)();
    setAnchorEl(null);
  };

  const handleAdd = () => {
    addFile('', '', 'Novo Arquivo');
    setOpen(false);
  };

  const handleUpload = () => {
    ipcRenderer.send('invoke_open_file');
    ipcRenderer.once('open_file', (e, path, code = '') => {
      if (path === false) {
        if (code != '') {
          alertShow('error', code);
        }
      } else {
        alertShow('success', 'Arquivo aberto');
        addFile(path, code);
      }
    });
    setOpen(false);
  };

  const handleChangeClock = (e,value) => {
   // console.log(e.target.value);
   // const value = parseInt(e.target.value);
    event('clock_change',[value]);
    setClock(value);
  }

  const handleAbout = () => { };

  return (
    <div className={classes.root}>
      <AppBar position='static' style={{ backgroundColor: '#191a21' }}>
        <Toolbar variant='dense'>
          <IconButton
            edge='start'
            className={classes.menuButton}
            color='inherit'
            aria-label='menu'
            onClick={handleDrawerOpen}
          >
            <MenuIcon />
          </IconButton>
          <UpdateIcon />
          <Slider
            onBlur={() => event('clock_change',[clock])}
            className={classes.slider}
            defaultValue={16}
            valueLabelDisplay='auto'
            step={1}
            marks
            min={1}
            max={20}
            onChange={(e,value) => setClock(value)}
            value={clock}
            style={{
              color: '#fff',
              width: 200,
              marginLeft: '1em',
              marginRight: '2em',
            }}
          />
          <Tooltip title='Expandir Macro (requestExpandMacros)'>
            <Button
              color='inherit'
              onClick={handlePlay('requestExpandMacros')}
              disabled={playing}
              className={classes.button}
            >
              <AllOutIcon
                style={{ color: !playing ? '#169dff' : '#169dff55' }}
              />
            </Button>
          </Tooltip>
          <Tooltip title='Executar (requestRun)'>
            <Button
              color='inherit'
              onClick={handlePlay('requestRun')}
              disabled={playing}
              className={classes.button}
            >
              <PlayArrowIcon
                style={{ color: !playing ? '#3dff3d' : '#3dff3d55' }}
              />
            </Button>
          </Tooltip>
          <Tooltip title={byStep ? 'Avan??ar (requestNextStep)' :'Avan??ar (requestRunBySteps)'}>
            <Button
              color='inherit'
              onClick={handlePlay(byStep ? 'requestNextStep' : 'requestRunBySteps')}
              disabled={playing}
              className={classes.button}
            >
              <SkipNextIcon />
            </Button>
          </Tooltip>
          <Tooltip title='Parar (requestKillProcess)'>
            <Button
              color='inherit'
              onClick={handlePlay('requestKillProcess')}
              disabled={!playing}
              className={classes.button}
            >
              <StopIcon style={{ color: playing ? '#ff6535' : '#ff653555' }} />
            </Button>
          </Tooltip>
          <Tooltip title='Debugar'>
            <Button
              color='inherit'
              onClick={(event) => setAnchorEl(event.currentTarget)}
              className={classes.button}
            >
              <BugReportIcon />
            </Button>
          </Tooltip>
        </Toolbar>
      </AppBar>
      <Drawer
        className={classes.menu}
        anchor={'left'}
        open={open}
        onClose={handleDrawerClose}
      >
        <ul>
          <li>
            <div
              className={classes.menuOption}
              aria-label='novo arquivo'
              onClick={handleAdd}
            >
              <DescriptionIcon
                style={{ fontSize: '1.5em', margin: '0 1em 0 .25em' }}
              />
              Novo arquivo
            </div>
          </li>
          <li>
            <div
              className={classes.menuOption}
              aria-label='menu'
              onClick={handleUpload}
            >
              <FolderOpenIcon
                style={{ fontSize: '1.5em', margin: '0 1em 0 .25em' }}
              />
              Abrir arquivo
            </div>
          </li>
          <li>
            <div
              className={classes.menuOption}
              aria-label='menu'
              onClick={handleAbout}
            >
              <InfoIcon
                style={{ fontSize: '1.5em', margin: '0 1em 0 .25em' }}
              />
              Sobre
            </div>
          </li>
        </ul>
      </Drawer>
      <Menu
        anchorEl={anchorEl}
        anchorOrigin={{ vertical: 'bottom', horizontal: 'right' }}
        id='debug'
        keepMounted
        //transformOrigin={{ vertical: 'top', horizontal: 'left' }}
        open={Boolean(anchorEl)}
        onClose={() => setAnchorEl(null)}
      >
        {requests.map((name) => (
          <MenuItem onClick={handleClickDebug(name)}>{name}</MenuItem>
        ))}
      </Menu>
    </div>
  );
};

/*

<TreeView
          color='primary'
          className={classes.root}
          defaultCollapseIcon={<ExpandMoreIcon />}
          defaultExpandIcon={<ChevronRightIcon />}
          multiSelect
        >
          <TreeItem nodeId='1' label='Applications'>
            <TreeItem nodeId='2' label='Calendar' />
            <TreeItem nodeId='3' label='Chrome' />
            <TreeItem nodeId='4' label='Webstorm' />
          </TreeItem>
        </TreeView>

*/

export default Header;
/*
SomaMem MACRO Mem1, Mem2
[1] mov AX,Mem1
[1] push AX
[1] ENDM
[1] l0:  SomaMem MACRO Mem1, Mem2
[1]
[1] l1:  mov AX,Mem1
[1]
[1] l2:  push AX
[1]
[1] l3:  ENDM
[1] size: 49
[1] entrou no set
[1] ------------------------------
[1] %c DEBUG: init
[1] ------------------------------
[1] SomaMem MACRO Mem1, Mem2
[1]
[1] ^
[1] curr char:   S
[1] SomaMem MACRO Mem1, Mem2
[1]
[1]  ^
[1] curr char:   o
[1] SomaMem MACRO Mem1, Mem2
[1]
[1]   ^
[1] curr char:   m
[1] SomaMem MACRO Mem1, Mem2
[1]
[1]    ^
[1] curr char:   a
[1] SomaMem MACRO Mem1, Mem2
[1]
[1]     ^
[1] curr char:   M
[1] SomaMem MACRO Mem1, Mem2
[1]
[1]      ^
[1] curr char:   e
[1] SomaMem MACRO Mem1, Mem2
[1]
[1]       ^
[1] curr char:   m
[1] SomaMem MACRO Mem1, Mem2
[1]
[1]        ^
[1] curr char:
[1] SomaMem MACRO Mem1, Mem2
[1]
[1]         ^
[1] curr char:   M
[1] t:  25, n:  -1
[1] ------------------------------
[1] %c DEBUG: somamem
[1] ------------------------------
[1] SomaMem MACRO Mem1, Mem2
[1]
[1]         ^
[1] curr char:   M
[1] SomaMem MACRO Mem1, Mem2
[1]
[1]          ^
[1] curr char:   A
[1] SomaMem MACRO Mem1, Mem2
[1]
[1]           ^
[1] curr char:   C
[1] SomaMem MACRO Mem1, Mem2
[1]
[1]            ^
[1] curr char:   R
[1] SomaMem MACRO Mem1, Mem2
[1]
[1]             ^
[1] curr char:   O
[1] SomaMem MACRO Mem1, Mem2
[1]
[1]              ^
[1] curr char:
[1] SomaMem MACRO Mem1, Mem2
[1]
[1]               ^
[1] curr char:   M
[1] qEnd Success
[1] t:  0, n:  0
[1] ------------------------------
[1] %c DEBUG: macro
[1] ------------------------------
[1] SomaMem MACRO Mem1, Mem2
[1]
[1]               ^
[1] curr char:   M
[1] SomaMem MACRO Mem1, Mem2
[1]
[1]                ^
[1] curr char:   e
[1] SomaMem MACRO Mem1, Mem2
[1]
[1]                 ^
[1] curr char:   m
[1] SomaMem MACRO Mem1, Mem2
[1]
[1]                  ^
[1] curr char:   1
[1] SomaMem MACRO Mem1, Mem2
[1]
[1]                   ^
[1] curr char:   ,
[1] SomaMem MACRO Mem1, Mem2
[1]
[1]                    ^
[1] curr char:
[1] t:  25, n:  -1
[1] ------------------------------
[1] %c DEBUG: mem1
[1] ------------------------------
[1] SomaMem MACRO Mem1, Mem2
[1]
[1]                   ^
[1] curr char:   ,
[1] SomaMem MACRO Mem1, Mem2
[1]
[1]                    ^
[1] curr char:
[1] qEnd Success
[1] t:  21, n:  59
[1] ------------------------------
[1] %c DEBUG: ,
[1] ------------------------------
[1] SomaMem MACRO Mem1, Mem2
[1]
[1]                     ^
[1] curr char:   M
[1] SomaMem MACRO Mem1, Mem2
[1]
[1]                      ^
[1] curr char:   e
[1] SomaMem MACRO Mem1, Mem2
[1]
[1]                       ^
[1] curr char:   m
[1] SomaMem MACRO Mem1, Mem2
[1]
[1]                        ^
[1] curr char:   2
[1] SomaMem MACRO Mem1, Mem2
[1]
[1]                         ^
[1] curr char:
[1]
[1] SomaMem MACRO Mem1, Mem2
[1]
[1]                          ^
[1] curr char:
[1] t:  25, n:  -1
[1] ------------------------------
[1] %c DEBUG: mem2
[1] ------------------------------
[1] SUCESSO
[1] entrou no set
[1] ------------------------------
[1] %c DEBUG: init
[1] ------------------------------
[1] mov AX,Mem1
[1]
[1] ^
[1] curr char:   m
[1] mov AX,Mem1
[1]
[1]  ^
[1] curr char:   o
[1] mov AX,Mem1
[1]
[1]   ^
[1] curr char:   v
[1] mov AX,Mem1
[1]
[1]    ^
[1] curr char:
[1] mov AX,Mem1
[1]
[1]     ^
[1] curr char:   A
[1] qEnd Success
[1] t:  3, n:  23
[1] mov AX,Mem1
[1]
[1] ^
[1] curr char:   m
[1] mov AX,Mem1
[1]
[1]  ^
[1] curr char:   o
[1] mov AX,Mem1
[1]
[1]   ^
[1] curr char:   v
[1] mov AX,Mem1
[1]
[1]    ^
[1] curr char:
[1] mov AX,Mem1
[1]
[1]     ^
[1] curr char:   A
[1] qEnd Success
[1] t:  3, n:  23
[1] entrou no set
[1] ------------------------------
[1] %c DEBUG: init
[1] ------------------------------
[1] push AX
[1]
[1] ^
[1] curr char:   p
[1] push AX
[1]
[1]  ^
[1] curr char:   u
[1] push AX
[1]
[1]   ^
[1] curr char:   s
[1] push AX
[1]
[1]    ^
[1] curr char:   h
[1] push AX
[1]
[1]     ^
[1] curr char:
[1] push AX
[1]
[1]      ^
[1] curr char:   A
[1] qEnd Success
[1] t:  3, n:  25
[1] push AX
[1]
[1] ^
[1] curr char:   p
[1] push AX
[1]
[1]  ^
[1] curr char:   u
[1] push AX
[1]
[1]   ^
[1] curr char:   s
[1] push AX
[1]
[1]    ^
[1] curr char:   h
[1] push AX
[1]
[1]     ^
[1] curr char:
[1] push AX
[1]
[1]      ^
[1] curr char:   A
[1] qEnd Success
[1] t:  3, n:  25
[1] entrou no set
[1] ------------------------------
[1] %c DEBUG: init
[1] ------------------------------
[1] ENDM
[1]
[1] ^
[1] curr char:   E
[1] ENDM
[1]
[1]  ^
[1] curr char:   N
[1] ENDM
[1]
[1]   ^
[1] curr char:   D
[1] ENDM
[1]
[1]    ^
[1] curr char:   M
[1] ENDM
[1]
[1]     ^
[1] curr char:
[1]
[1] ENDM
[1]
[1]      ^
[1] curr char:
[1] qEnd Success
[1] t:  1, n:  3
[1] ------------------------------
[1] %c DEBUG: endm
[1] ------------------------------
[1] SUCESSO
[1] -> SomaMem MACRO Mem1, Mem2
[1]
[1] -> mov AX,Mem1
[1]
[1] -> push AX
[1]
[1] -> ENDM
[1]
[1] Erro: macro n foi definida direito (FIM DO ARQUIVO)
[1] ### START ...



*/
