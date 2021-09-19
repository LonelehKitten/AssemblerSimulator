import React, { useEffect } from 'react';
import { makeStyles } from '@material-ui/core/styles';
import AppBar from '@material-ui/core/AppBar';
import Toolbar from '@material-ui/core/Toolbar';
import Typography from '@material-ui/core/Typography';
import Button from '@material-ui/core/Button';
import IconButton from '@material-ui/core/IconButton';
import Drawer from '@material-ui/core/Drawer';
import TreeView from '@material-ui/lab/TreeView';
import TreeItem from '@material-ui/lab/TreeItem';

import PlayArrowIcon from '@material-ui/icons/PlayArrow';
import StopIcon from '@material-ui/icons/Stop';
import SkipNextIcon from '@material-ui/icons/SkipNext';
import MenuIcon from '@material-ui/icons/Menu';
import ExpandMoreIcon from '@material-ui/icons/ExpandMore';
import ChevronRightIcon from '@material-ui/icons/ChevronRight';

import { useContext } from '../utils/context';

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
}));

const Header = () => {
  const {
    setConsoleFlag,
    currentFile,
    setPlayButtonPressed,
    playing,
    setPlaying,
  } = useContext();
  const classes = useStyles();

  const [open, setOpen] = React.useState(false);

  const handleDrawerOpen = () => {
    setOpen(true);
  };

  const handleDrawerClose = () => {
    setOpen(false);
  };

  const handlePlay = () => {
    // play_expandMacros
    if (
      currentFile !== null &&
      currentFile?.code !== null &&
      currentFile?.code !== ''
    ) {
      ipcRenderer.send('play_expandMacros', currentFile.code);
      setPlayButtonPressed('pressed');
      setConsoleFlag(true);
      setPlaying(true);
      console.log(currentFile.code);
    }
  };

  let playButton = (
    <Button color='inherit' onClick={handlePlay}>
      <PlayArrowIcon />
    </Button>
  );

  useEffect(() => {
    if (playing) {
      console.log('mudou o botão');
      playButton = (
        <Button color='inherit' onClick={handlePlay} disabled>
          <PlayArrowIcon />
        </Button>
      );
    }
  }, [playing]);

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
          <Typography variant='h6' className={classes.title}>
            Assembler Simulator
          </Typography>
          {playButton}
          <Button color='inherit'>
            <SkipNextIcon />
          </Button>
          <Button color='inherit'>
            <StopIcon />
          </Button>
        </Toolbar>
      </AppBar>
      <Drawer anchor={'left'} open={open} onClose={handleDrawerClose}>
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
      </Drawer>
    </div>
  );
};

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
