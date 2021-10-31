import React from 'react';
import { makeStyles } from '@material-ui/core/styles';
import CloseIcon from '@material-ui/icons/Close';

import FiberManualRecordIcon from '@material-ui/icons/FiberManualRecord';
import Tooltip from '@material-ui/core/Tooltip';
import { useContext } from '../../utils/context';

const useStyles = makeStyles((theme) => ({
  isSave: {
    fontSize: 15,
    marginRight: 5,
  },
  deleted: {
    color: 'white',
    fontSize: 18,
    marginLeft: 5,
  },
  selected: {
    backgroundColor: '#21222c !important',
    borderTop: '1px solid hotpink !important',
    '& .MuiTab-textColorInherit.Mui-selected': {},
  },
}));
const Item = (
  { label, onChange, isSave, onClick, indicator, value, selected, fileName },
  ref
) => {
  const { listFiles, setListFiles, currentFile, changeFile } = useContext();
  const classes = useStyles();

  const handleClick = (event) => {
    if (onChange) onChange(event, value);
    if (onClick) onClick(event);
  };

  const handleClose = async () => {
    if (!isSave || confirm('Você não salvou esse arquivo, deseja continua?')) {
      const newListFiles = { ...listFiles };
      delete newListFiles[value];
      setListFiles(newListFiles);
      if (currentFile?.id == value) changeFile(null);
    }
  };

  return (
    <Tooltip title={fileName} placement='bottom'>
      <button
        className={
          'MuiButtonBase-root MuiTab-root MuiTab-textColorInherit ' +
          (selected ? classes.selected : '')
        }
        ref={ref}
        onClick={handleClick}
        role='tab'
        aria-selected={selected}
        tabIndex={selected ? 0 : -1}
      >
        {isSave && <FiberManualRecordIcon className={classes.isSave} />}

        <span className='MuiTab-wrapper'>{label}</span>

        <Tooltip title='Fechar' placement='bottom'>
          <CloseIcon onClick={handleClose} className={classes.deleted} />
        </Tooltip>
        {indicator}
      </button>
    </Tooltip>
  );
};
export default React.forwardRef(Item);
