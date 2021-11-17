import MenuItem from '@material-ui/core/MenuItem';
import { useContext } from "../../utils/context";
import {file} from '../../utils';
import Divider from '@material-ui/core/Divider';
import event from '../../utils/event';

const {ipcRenderer} = window.electron;
const File = ({onClose}) => {
    const { addFile, alertShow, currentFile, setCode, setListFiles, changeFile, setCurrentFile, setTreeFiles } = useContext();

    const handleOpenFile = () => {
        file.load({alertShow,addFile});
        onClose();
    };

    const handleSaveFile = () => {
        file.save({alertShow, currentFile, setCode, setListFiles});
        onClose();
    };

    const handleOpenDirectory = () => {
        file.getTree({setTreeFiles});
        onClose();
    }

    const handleCloseFile = () => {
        file.close({setListFiles,changeFile,currentFile});
        onClose();
    };

    const handleCloseAllFile = () => {
        setListFiles([]);
        changeFile(null);
        onClose();
    }

    const handleNewFile = () => {
        addFile('', '', 'Novo Arquivo')
        onClose();
    }

    const handleClose = () => {
        event("windowsAction", ["close"]);
    }
    return (
        <>
            <MenuItem onClick={handleNewFile}>Novo Arquivo</MenuItem>
            <MenuItem onClick={handleOpenFile}>Abrir Arquivo</MenuItem>
            <MenuItem onClick={handleSaveFile}>Salvar Arquivo</MenuItem>
            <MenuItem onClick={handleOpenDirectory}>Abrir Diretório</MenuItem>
            <Divider />
            <MenuItem onClick={handleCloseFile}>Fechar Arquivo</MenuItem>
            <MenuItem onClick={handleCloseAllFile}>Fechar Todos os Arquivos</MenuItem>
            <Divider />
            <MenuItem onClick={handleClose}>Sair</MenuItem>
        </>
    );
}

export default File;