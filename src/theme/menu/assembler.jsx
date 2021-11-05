import MenuItem from '@material-ui/core/MenuItem';
import ListItemIcon from '@material-ui/core/ListItemIcon';
import CheckIcon from '@material-ui/icons/Check';
import Divider from '@material-ui/core/Divider';


const { ipcRenderer } = window.electron;
const Assembler = ({ onClose }) => {

    return (
        <>
            <MenuItem>
                <ListItemIcon>
                    <CheckIcon color="inherit" fontSize="small" />
                </ListItemIcon>
                Modo 1
            </MenuItem>
            <MenuItem>
                <ListItemIcon>
                </ListItemIcon>
                Modo 2
            </MenuItem>
        </>
    );
}

export default Assembler;