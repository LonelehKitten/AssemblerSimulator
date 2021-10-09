import React, { useRef, useState } from 'react';
import Menu from '@material-ui/core/Menu';
import MenuItem from '@material-ui/core/MenuItem';
import { makeStyles } from '@material-ui/core/styles';
import TabContainer from '@material-ui/core/Tabs';
import Tab from '@material-ui/core/Tab';
import Tooltip from '@material-ui/core/Tooltip';

import AddIcon from '@material-ui/icons/Add';

import { useContext } from '../../utils/context';

import Item from './item';

const useStyles = makeStyles((theme) => ({
  root: {
    backgroundColor: '#282a36 ',
    color: '#fff',
    minHeight: '2rem',
    '& 	.MuiTab-root': {
      boxShadow: '0 3px 6px rgba(0,0,0,0.16), 0 3px 6px rgba(0,0,0,0.23)',
      padding: '0 12px',
      minHeight: '2rem',
    },
  },
  plus: {
    minWidth: 'auto',
  },
}));

const { ipcRenderer } = window.electron;

const Tabs = ({ value, onChange, listFiles }) => {
  const { addFile, currentID, alertShow } = useContext();
  const classes = useStyles();

  const inputFile = useRef(null);
  const [anchorEl, setAnchorEl] = useState(null);

  const onChangeFile = (event) => {
    event.stopPropagation();
    event.preventDefault();
    const file = event.target.files[0];
    const reader = new FileReader();
    reader.readAsText(file, 'UTF-8');
    reader.onload = function (evt) {
      const code = evt.target.result;
      alertShow('success', 'Arquivo aberto');
      addFile(file.path, code);
    };
    reader.onerror = function (evt) {
      alertShow('error', 'Erro ao ler o arquivo');
    };
    event.target.value = '';
  };

  const handleClick = (event) => {
    setAnchorEl(event.currentTarget);
  };

  const handleClose = () => {
    setAnchorEl(null);
  };

  const handleAdd = () => {
    addFile('', '', 'Novo Arquivo');
    setAnchorEl(null);
  };

  const handleUpload = () => {
    setAnchorEl(null);
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
  };

  return (
    <div
      style={{
        boxShadow: 'inset 0 0 1em balck',
      }}
    >
      <TabContainer
        value={currentID || ''}
        indicatorColor='secondary'
        textColor='inherit'
        onChange={onChange}
        aria-label='disabled tabs example'
        variant='scrollable'
        scrollButtons='auto'
        className={classes.root}
      >
        <Tab
          value=''
          onClick={handleClick}
          icon={<AddIcon />}
          className={classes.plus}
        />
        {Object.entries(listFiles).map(([id, file], key) => (
          <Item
            key={key}
            isSave={file.isSave}
            value={id}
            label={file.name.split('/')[file.name.split('/').length - 1]}
            fileName={file.name}
          />
        ))}
      </TabContainer>
      <Menu
        id='simple-menu'
        anchorEl={anchorEl}
        keepMounted
        open={Boolean(anchorEl)}
        onClose={handleClose}
      >
        <MenuItem onClick={handleAdd}>Novo Arquivo</MenuItem>
        <MenuItem onClick={handleUpload}>Enviar Arquivo</MenuItem>
      </Menu>
      <input
        accept='.asm'
        type='file'
        id='file'
        ref={inputFile}
        style={{ display: 'none' }}
        onChange={onChangeFile}
      />
    </div>
  );
};
export default Tabs;
