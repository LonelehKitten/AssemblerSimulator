import MenuItem from '@material-ui/core/MenuItem';
import ListItemIcon from '@material-ui/core/ListItemIcon';
import CheckIcon from '@material-ui/icons/Check';
import Divider from '@material-ui/core/Divider';


const { ipcRenderer } = window.electron;
const Help = ({ onClose }) => {

    return (
        <>
            <MenuItem>Ajuda</MenuItem>
            <MenuItem>Colaboradores</MenuItem>
            <MenuItem>Versão 1.0.0</MenuItem>
        </>
    );
}

export default Help;