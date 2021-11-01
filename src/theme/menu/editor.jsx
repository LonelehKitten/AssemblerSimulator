import MenuItem from '@material-ui/core/MenuItem';
import ListItemIcon from '@material-ui/core/ListItemIcon';
import CheckIcon from '@material-ui/icons/Check';
import Divider from '@material-ui/core/Divider';


const { ipcRenderer } = window.electron;
const Editor = ({ onClose }) => {

    return (
        <>
            <MenuItem>
                <ListItemIcon>
                    <CheckIcon color="inherit" fontSize="small" />
                </ListItemIcon>
                AutoComplete
            </MenuItem>
            <MenuItem>
                <ListItemIcon>
                </ListItemIcon>
                AutoComplete
            </MenuItem>
        </>
    );
}

export default Editor;