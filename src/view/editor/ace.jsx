import AceEditor from 'react-ace';
import { makeStyles } from '@material-ui/core/styles';
import 'ace-builds/src-noconflict/mode-assembly_x86';
import 'ace-builds/src-noconflict/theme-dracula';
import 'ace-builds/src-noconflict/ext-language_tools';

import { useContext } from '../../utils/context';
import { useEffect, useState } from 'react';

const { ipcRenderer } = window.electron;

const Ace = ({ onChange }) => {
  const {
    listFiles,
    currentID,
    currentFile,
    setCode,
    alertShow,
    setListFiles,
  } = useContext();
  const [teste, setTeste] = useState(true);

  const useStyles = makeStyles((theme) => ({
    '@global': {
      '*::-webkit-scrollbar': {
        width: '0.4em',
        height: '0.4em',
      },
      '*::-webkit-scrollbar-track': {
        '-webkit-box-shadow': 'inset 0 0 6px rgba(0,0,0,0.02)',
      },
      '*::-webkit-scrollbar-thumb': {
        backgroundColor: '#191b24',
        outline: '1px solid ',
      },
    },
  }));

  const handleChange = (value) => {
    currentFile.isSave = true;
    setCode(value);
  };

  const handleSave = (e) => {
    if (e.keyCode == 83 && e.ctrlKey) {
      if (currentFile.isSave) {
        ipcRenderer.send('invoke_save_file', JSON.stringify(currentFile));
        ipcRenderer.once('save_file', (e, success, path) => {
          if (success) {
            setCode(currentFile.code, true);
            if (currentFile.path == '') {
              currentFile.path = path;
              currentFile.name = path.split('\\').slice(-1)[0];
              setListFiles({ ...listFiles, ...{ [currentID]: currentFile } });
            }
            alertShow('success', 'Arquivo Salvo');
          } else {
            alertShow('danger', 'Erro ao salvar o arquivo');
          }
        });
      }
    }
  };

  const stopKey = (event) => {
    /*if (event.keyCode == 83 && event.ctrlKey) {
            event.preventDefault();
            event.stopPropagation();
            return false;
        }*/
  };

  const classes = useStyles();

  return (
    <div onKeyUp={handleSave} style={{ width: '100%', height: '100%' }}>
      {/** Gambiarra */}

      <AceEditor
        commands={[
          {
            name: 'save',
            bindKey: { win: 'Ctrl-S', mac: 'Cmd-S' },
            exec: (e) => {
              //  handleSave(e);
            },
          },
        ]}
        readOnly={currentFile === null}
        mode='assembly_x86'
        theme='dracula'
        placeholder={currentFile === null ? 'Crie um arquivo . . .' : ''}
        onChange={handleChange}
        name='UNIQUE_ID_OF_DIV'
        value={currentFile?.code || ''}
        style={{
          width: '100%',
          height: 'calc(100% - 2rem)',
          fontFamily: 'Share Tech Mono',
        }}
        editorProps={{ $blockScrolling: true }}
        setOptions={{
          printMargin: 50,
          showInvisibles: true,
          fontSize: 20,
          enableLiveAutocompletion: true,
          tabSize: 2,
          useSoftTabs: true,
        }}
      />
    </div>
  );
};

export default Ace;
