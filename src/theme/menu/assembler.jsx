import { useState } from 'react';
import MenuItem from '@material-ui/core/MenuItem';
import ListItemIcon from '@material-ui/core/ListItemIcon';
import CheckIcon from '@material-ui/icons/Check';
import Divider from '@material-ui/core/Divider';


const { ipcRenderer } = window.electron;

const options = [
    {
        name: "2 Passos",
        value: 0
    },
    {
        name: "Load-and-Go",
        value: 1
    }
];

const Assembler = ({ onClose }) => {

    const [checked, setChecked] = useState("");

    const handleChangeCheck = (value) => (e) => {
        setChecked(value);
        setTimeout(onClose,100);
    }

    return (
        <>
            {options.map((item) =>
                <MenuItem button onClick={handleChangeCheck(item.value)}>
                    <ListItemIcon>
                        {item.value == checked && <CheckIcon color="inherit" fontSize="small" />}
                    </ListItemIcon>
                    {item.name}
                </MenuItem>
            )}
        </>
    );
}

export default Assembler;