import { useState } from 'react';
import MenuItem from '@material-ui/core/MenuItem';
import ListItemIcon from '@material-ui/core/ListItemIcon';
import CheckIcon from '@material-ui/icons/Check';
import Menu from '../../components/menu';
import { useContext } from '../../utils';
import event from '../../utils/event';

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
    const { modeAssembler, setModeAssembler } = useContext();
    //const [checked, setChecked] = useState("");

    const handleChangeCheck = (value) => (e) => {
        setModeAssembler(value);
        event('play', ['requestModeAssembler',[value]]);
    }

    return (
        <Menu label="Assembler">
            {options.map((item) =>
                <MenuItem button onClick={handleChangeCheck(item.value)}>
                    <ListItemIcon>
                        {item.value == modeAssembler && <CheckIcon color="inherit" fontSize="small" />}
                    </ListItemIcon>
                    {item.name}
                </MenuItem>
            )}
        </Menu>
    );
}

export default Assembler;