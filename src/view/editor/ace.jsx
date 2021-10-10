import AceEditor from 'react-ace';
import 'ace-builds/src-noconflict/mode-assembly_x86';
import 'ace-builds/src-noconflict/theme-dracula';
import 'ace-builds/src-noconflict/ext-language_tools';

import { useContext } from '../../utils/context';

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

  return (
    <div onKeyUp={handleSave} style={{ width: '100%', height: '100%' }}>
      {/** Gambiarra [ tem que ver isso aqui ] */}

      <AceEditor
        readOnly={currentFile === null}
        mode='assembly_x86'
        theme='dracula'
        placeholder={currentFile === null ? 'Crie um arquivo . . .' : ''}
        onChange={handleChange}
        name='editor'
        value={currentFile?.code || ''}
        style={{
          boxShadow:
            'inset  0 -3px 6px rgba(0,0,0,0.16), 0 -3px 6px rgba(0,0,0,0.23)',
          width: '100%',
          height: 'calc(100% - 2rem - 32px)',
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
          cursorStyle: 'ace',
          showGutter: true,
          highlightGutterLine: true,
          //highlightActiveLine: false,
        }}
      />
    </div>
  );
};

export default Ace;
