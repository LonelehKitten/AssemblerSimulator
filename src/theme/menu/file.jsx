import MenuItem from '@material-ui/core/MenuItem';
import { useContext } from "../../utils/context";
import {file} from '../../utils';
import Divider from '@material-ui/core/Divider';
import event from '../../utils/event';
import Menu from '../../components/menu';

const File = () => {
    const { addFile, alertShow, currentFile, setCode, setListFiles, changeFile, setCurrentFile, setTreeFiles } = useContext();

    const handleOpenFile = () => {
        file.load({alertShow,addFile});
    };

    const handleSaveFile = () => {
        file.save({alertShow, currentFile, setCode, setListFiles});
    };

    const handleOpenDirectory = () => {
        file.getTree({setTreeFiles,alertShow});
    }

    const handleCloseFile = () => {
        file.close({setListFiles,changeFile,currentFile});
    };

    const handleCloseAllFile = () => {
        setListFiles([]);
        changeFile(null);
    }

    const handleNewFile = () => {
        addFile('', '', 'Novo Arquivo')
    }

    const handleClose = () => {
        event("windowsAction", ["close"]);
    }
    return (
        <Menu label="Arquivo">
            <MenuItem onClick={handleNewFile}>Novo Arquivo</MenuItem>
            <MenuItem onClick={handleOpenFile}>Abrir Arquivo</MenuItem>
            <MenuItem onClick={handleSaveFile}>Salvar Arquivo</MenuItem>
            <MenuItem onClick={handleOpenDirectory}>Abrir Diretório</MenuItem>
            <Divider />
            <MenuItem onClick={handleCloseFile}>Fechar Arquivo</MenuItem>
            <MenuItem onClick={handleCloseAllFile}>Fechar Todos os Arquivos</MenuItem>
            <Divider />
            <MenuItem onClick={handleClose}>Sair</MenuItem>
        </Menu>
    );
}

export default File;