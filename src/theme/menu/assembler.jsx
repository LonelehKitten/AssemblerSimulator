import { useState } from 'react';
import MenuItem from '@material-ui/core/MenuItem';
import ListItemIcon from '@material-ui/core/ListItemIcon';
import CheckIcon from '@material-ui/icons/Check';
import Menu from '../../components/menu';

const options = [
    {
        name: "Básico",
        value: 0
    },
    {
        name: "Completo",
        value: 1
    }
];

const Assembler = () => {

    const [checked, setChecked] = useState("");

    const handleChangeCheck = (value) => (e) => {
        setChecked(value);
    }

    return (
        <Menu label="Assembler">
            {options.map((item) =>
                <MenuItem button onClick={handleChangeCheck(item.value)}>
                    <ListItemIcon>
                        {item.value == checked && <CheckIcon color="inherit" fontSize="small" />}
                    </ListItemIcon>
                    {item.name}
                </MenuItem>
            )}
        </Menu>
    );
}

export default Assembler;