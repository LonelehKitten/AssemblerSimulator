import { useMemo } from "react";
import { useContext } from "../utils/context";
import MenuContainer from '../components/menu';

const { ipcRenderer } = window.electron;


const Menu = ({ type, anchorEl, onClose,handleButton, ...props }) => {
    const { addFile, alertShow, currentFile, setCode, setListFiles, changeFile, setCurrentFile } = useContext();

    const handleUpload = () => {
        ipcRenderer.send('invoke_open_file');
        ipcRenderer.once('open_file', (e, path, code = '') => {
            if (path === false) {
                if (code != '') {
                    alertShow('error', code);
                }
            } else {
                alertShow('success', 'Arquivo aberto');
                console.log(path,code);
                addFile(path, code);
            }
        });
    };

    const handleSave = () => {
        if (currentFile?.isSave) {
            ipcRenderer.send('invoke_save_file', JSON.stringify(currentFile));
            ipcRenderer.once('save_file', (e, success, path) => {
                if (success) {
                    setCode(currentFile.code, true);
                    if (currentFile.path == '') {
                        currentFile.path = path;
                        currentFile.name = path.split('\\').slice(-1)[0];
                        setListFiles((listFiles) => ({ ...listFiles, ...{ [currentFile.id]: currentFile } }));
                    }
                    alertShow('success', 'Arquivo Salvo');
                } else {
                    alertShow('danger', 'Erro ao salvar o arquivo');
                }
            });
        }
    };
    const handleCloseFile = async () => {
        console.log(currentFile);
        if(currentFile == null) return;
        if (!currentFile.isSave || confirm('Você não salvou esse arquivo, deseja continua?')) {
            const id = currentFile.id;
            setListFiles((listFiles) => {
                const newListFiles = { ...listFiles };
                delete newListFiles[id];
    
                return newListFiles;
            });
            changeFile(null);
        }
    };

    const handleCloseAllFile = () => {
        setListFiles([]);
        changeFile(null);
    }

    const options = useMemo(() => ({
        file: [
            { label: "Novo Arquivo", onClick: () => addFile('', '', 'Novo Arquivo') },
            { label: "Abrir Arquivo", onClick: handleUpload },
            { label: "Salvar Arquivo", onClick: handleSave },
            { divider: true },
            { label: "Fechar o Arquivo", onClick: handleCloseFile },
            { label: "Fechar Todos Arquivo", onClick: handleCloseAllFile },
            { divider: true },
            { label: "Sair", onClick: handleButton("close") }
        ],

    }), []);

    return <MenuContainer
        anchorEl={anchorEl}
        //                keepMounted
        open={Boolean(anchorEl)}
        {...props}
        onClose={onClose}
        style={{ marginTop: 18 }}
        anchorOrigin={{
            horizontal: 'center'
        }}
        transformOrigin={{
            vertical: 'bottom',
            horizontal: 'center',
        }}
        options={options[type] ?? []}
    />;
}

export default Menu;