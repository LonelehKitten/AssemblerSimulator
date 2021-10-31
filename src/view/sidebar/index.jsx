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
import { useContext } from '../../utils/context';
import { isEmpty } from '../../utils';
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
  } = useContext();

  const [anchorEl, setAnchorEl] = useState(null);

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
      // Habilita ou não o "Playing"
      if (
        type == 'simulate' ||
        type == 'requestEndTest' ||
        type == 'requestTest' ||
        type == 'requestKillProcess'
      )
        setPlaying(false);
      else setPlaying(true);

      const params = [];

      // Adicionar as instruções
      if (['requestRun', 'requestRunBySteps'].includes(type)) {
        let arr = getBytecodeFromEditor();
        console.log(arr);
        params.push(arr);
      }
      if (
        [
          'requestExpandMacros',
          'requestAssembleAndRun',
          'requestAssembleAndRunBySteps',
        ].includes(type)
      ) {
        params.push(currentFile?.code);
      }
      if (
        [
          'requestAssembleAndRun',
          'requestAssembleAndRunBySteps',
          'requestRun',
          'requestRunBySteps',
        ].includes(type)
      ) {
        params.push(memoryToBytes);
      }
      if (
        [
          'requestRunBySteps',
          'requestAssembleAndRunBySteps',
          'requestNextStep',
        ].includes(type)
      ) {
        console.log('Header|BySteyp', type, true);
        setByStep(true);
      } else {
        console.log('Header|BySteyp', type, false);
        setByStep(false);
      }
      event('play', [type, params]);
    }
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
