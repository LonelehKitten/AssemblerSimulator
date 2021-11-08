import List from '@material-ui/core/List';
import ListItem from '@material-ui/core/ListItem';
import Collapse from '@material-ui/core/Collapse';
import MenuItem from '@material-ui/core/MenuItem';
import ListItemIcon from '@material-ui/core/ListItemIcon';
import CheckIcon from '@material-ui/icons/Check';
import Divider from '@material-ui/core/Divider';
import { useState } from 'react';


const { ipcRenderer } = window.electron;
const Editor = ({ onClose }) => {

    const [open,setOpen] = useState(false);
    console.log(open);
    return (
        <>
            <MenuItem>
                <ListItemIcon>
                    <CheckIcon color="inherit" fontSize="small" />
                </ListItemIcon>
                AutoComplete
            </MenuItem>
            <MenuItem button onClick={() => setOpen(!open)}>
                <ListItemIcon>
                </ListItemIcon>
                AutoComplete
            </MenuItem>
        </>
    );
}

export default Editor;