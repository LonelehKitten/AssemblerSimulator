import MenuItem from '@material-ui/core/MenuItem';
import ListItemIcon from '@material-ui/core/ListItemIcon';
import CheckIcon from '@material-ui/icons/Check';
import { useEffect, useState } from 'react';

import Menu from '../../components/menu';
import event from '../../utils/event';
import { useContext } from '../../utils';

const Editor = () => {
    const { editorConfig, changeEditorConfig } = useContext();

    const [listFonts, setListFonts] = useState([]);

    useEffect(() => {
        event("getFonts", [], (e, fonts) => {
            setListFonts(["Share Tech Mono", ...fonts]);
        })
    }, []);
    const handleChangeEditorConfig = (name, value) => () => {
        changeEditorConfig(name, value);
    }

    const listSizes = [12,16,20, 24,28,32];

    return (
        <Menu label="Editor">
            <Menu label="Tamanho" submenu>
                {listSizes.map((item) => (
                    <MenuItem onClick={handleChangeEditorConfig("fontSize", item)}>
                        <ListItemIcon>
                            {item == editorConfig.fontSize && <CheckIcon color="inherit" fontSize="small" />}
                        </ListItemIcon>
                        {item}px
                    </MenuItem>
                ))}
            </Menu>
            <Menu label="Fonte" submenu>
                {listFonts.map((item) => (
                    <MenuItem onClick={handleChangeEditorConfig("fontFamily", item)}>
                        <ListItemIcon>
                            {item == editorConfig.fontFamily && <CheckIcon color="inherit" fontSize="small" />}
                        </ListItemIcon>
                        {item}
                    </MenuItem>
                )
                )}
            </Menu>
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