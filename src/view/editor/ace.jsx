import React, { forwardRef } from 'react';
import AceEditor from 'react-ace';
import 'ace-builds/src-noconflict/mode-assembly_x86';
import 'ace-builds/src-noconflict/theme-dracula';
import 'ace-builds/src-noconflict/ext-language_tools';

import { file, useContext } from '../../utils';

const { ipcRenderer } = window.electron;

const Ace = ({ onChange }, ref) => {
  const {
    listFiles,
    currentFile,
    setCode,
    alertShow,
    setListFiles,
    editorConfig,
    addFile
  } = useContext();

  const handleChange = (value) => {
    currentFile.isSave = true;
    setCode(value);
  };

  //AceEditor.

  const handleSave = (e) => {
    if (e.keyCode == 83 && e.ctrlKey) {
      file.save({ alertShow, currentFile, setCode, setListFiles });
    }else if (e.keyCode == 79 && e.ctrlKey) {
      file.load({alertShow,addFile});
    }
  };

  return (
    <div
      onKeyUp={handleSave}
      style={{ width: '100%', height: 'calc(100% - 4rem)' }}
    >
      {/** Gambiarra [ tem que ver isso aqui ] */}

      <AceEditor
        ref={ref}
        readOnly={currentFile === null}
        mode='assembly_x86'
        theme='dracula'
        placeholder={currentFile === null ? 'Crie um arquivo . . .' : ''}
        onChange={handleChange}
        name='editor'
        value={currentFile?.code || ''}
        style={{
          boxShadow: 'inset  0 -3px 6px rgba(0,0,0,0.16), 0 -3px 6px rgba(0,0,0,0.23)',
          width: '100%',
          height: '100%',
          fontFamily: editorConfig.fontFamily,
        }}
        editorProps={{ $blockScrolling: true }}
        setOptions={{
          printMargin: 50,
          showInvisibles: true,
          fontSize: editorConfig.fontSize,
          enableLiveAutocompletion: true,
          tabSize: 2,
          useSoftTabs: true,
          cursorStyle: 'ace',
          showGutter: true,
          highlightGutterLine: true,
          //highlightActiveLine: false,
        }}
      />
    </div>
  );
};

export default forwardRef(Ace);
