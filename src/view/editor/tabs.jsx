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
        {Object.entries(listFiles).map(([id, file], key) => (
          <Item
            key={key}
            isSave={file.isSave}
            value={id}
            label={file.name.split('/')[file.name.split('/').length - 1]}
            fileName={file.path}
          />
        ))}
      </TabContainer>
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
