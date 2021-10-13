import React, {useState} from 'react';
import { makeStyles } from '@material-ui/core/styles';

const useStyles = makeStyles((theme) => ({
  dragger: {
    padding: '0px 0 0',
    borderTop: '1px solid #363a52',
    display: 'inline-block',
    position: 'relative',
    zIndex: 100,
  },
  horizontal: {
    backgroundColor: 'transparent',
    borderTop: '1px solid #e094ff',
    cursor: 'n-resize',
    //left: 0,
    //right: 0,
    width: '100%',
    height: '5px',
  },
  vertical: {
    backgroundColor: 'transparent',
    cursor: 'ew-resize',
    width: '5px',
    height: '100%'
  },
}));

const Dragger = ({ onMouse, orientation = 'vertical', style={} }) => {
  const classes = useStyles();
  const orientationClasse =
    orientation == 'horizontal' ? classes.horizontal : classes.vertical;

  const handleMouseDown = (e) => {
    document.addEventListener('mouseup', handleMouseUp, true);
    document.addEventListener('mousemove', onMouse, true);
  };

  const handleMouseUp = () => {
    document.removeEventListener('mouseup', handleMouseUp, true);
    document.removeEventListener('mousemove', onMouse, true);
  };

  return (
    <div
      onMouseDown={handleMouseDown}
      className={classes.dragger + ' ' + orientationClasse}
      style={style}
    />
  );
};

export default Dragger;
