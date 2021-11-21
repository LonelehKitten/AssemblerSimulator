import { makeStyles } from '@material-ui/core/styles';
import Toolbar from '@material-ui/core/Toolbar';
import React, { useState, useEffect } from 'react';
import Button from '@material-ui/core/Button';
import Tooltip from '@material-ui/core/Tooltip';
import Menu from '@material-ui/core/Menu';
import MenuItem from '@material-ui/core/MenuItem';

import PlayArrowIcon from '@material-ui/icons/PlayArrow';
import StopIcon from '@material-ui/icons/Stop';
import SkipNextIcon from '@material-ui/icons/SkipNext';
import AllOutIcon from '@material-ui/icons/AllOut';
import BugReportIcon from '@material-ui/icons/BugReport';
import event from '../../utils/event';
import { isEmpty, useContext, electron } from '../../utils';

const useStyles = makeStyles((theme) => ({
  root: {
    display: 'block',
    width: '50px',
    height: 'calc(100vh - 36px - 1.5em)',
    backgroundColor: '#21222c',
    zIndex: 9,
    '& .MuiButton-root': {
      minWidth: 50,
      height: 50,
      padding: 0,
    },
  },
  menuRoot: {
    '& .MuiMenu-paper': {
      //backgroundColor: "#535358",
      backgroundColor: '#292b35',
      color: '#f1f1f1',
      '& .MuiMenuItem-root:hover': {
        backgroundColor: 'rgba(255,255,255,0.2)',
      },
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

const SideBar = () => {
  const classes = useStyles();
  const {
    memory,
    currentFile,
    addFile,
    alertShow,
    playing,
    setPlaying,
    byStep,
    setByStep,
    treeFiles,
    modeAssembler
  } = useContext();

  const [anchorEl, setAnchorEl] = useState(null);

  const handlePlay = (type) => () => {
    electron.request.play({type,memory, currentFile, setPlaying, setByStep, treeFiles, modeAssembler });
  };

  const handleClickDebug = (type) => () => {
    handlePlay(type)();
    setAnchorEl(null);
  };
  return (
    <div className={classes.root}>
      <Tooltip title='Expandir Macro (requestExpandMacros)'>
        <Button
          color='inherit'
          onClick={handlePlay('requestExpandMacros')}
          disabled={playing}
          className={classes.button}
        >
          <AllOutIcon style={{ color: !playing ? '#169dff' : '#169dff55' }} />
        </Button>
      </Tooltip>
      <Tooltip title='Executar (requestAssembleAndRun)'>
        <Button
          color='inherit'
          onClick={handlePlay('requestAssembleAndRun')}
          disabled={playing}
          className={classes.button}
        >
          <PlayArrowIcon
            style={{ color: !playing ? '#3dff3d' : '#3dff3d55' }}
          />
        </Button>
      </Tooltip>
      <Tooltip
        title={
          byStep ? 'Avançar (requestNextStep)' : 'Avançar (requestRunBySteps)'
        }
      >
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
      <Menu
        anchorEl={anchorEl}
        anchorOrigin={{ vertical: 'bottom', horizontal: 'right' }}
        id='debug'
        keepMounted
        className={classes.menuRoot}
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

export default SideBar;
