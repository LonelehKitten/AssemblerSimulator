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
  const { currentFile, setPlayButtonPressed, playing, setPlaying } =
    useContext();
  const classes = useStyles();

  const [pressed, setPressed] = React.useState(true);

  const [open, setOpen] = React.useState(false);

  const handleDrawerOpen = () => {
    setOpen(true);
  };

  const handleDrawerClose = () => {
    setOpen(false);
  };

  const handlePlay = () => {
    // play_expandMacros
    if (currentFile !== null && currentFile?.code !== null) {
      ipcRenderer.send('play_expandMacros', currentFile.code);
      setPlayButtonPressed('pressed');
      setPlaying(true);
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
