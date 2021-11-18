import MenuItem from '@material-ui/core/MenuItem';

import Menu from '../../components/menu';

const Editor = () => {

    return (
        <Menu label="Editor">
            <Menu label="Fonte" submenu>
                <MenuItem>12px</MenuItem>
                <MenuItem>14px</MenuItem>
                <MenuItem>16px</MenuItem>
                <MenuItem>18px</MenuItem>
                <MenuItem>20px</MenuItem>
                <MenuItem>22px</MenuItem>
            </Menu>
            <MenuItem>Colaboradores</MenuItem>
            <MenuItem>Versão Beta</MenuItem>
        </Menu>
    );
    /*
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
    );*/
}

export default Editor;