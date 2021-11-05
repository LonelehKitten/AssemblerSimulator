import React, { useState } from 'react';
import { makeStyles } from '@material-ui/core/styles';
import AppBar from '@material-ui/core/AppBar';
import Toolbar from '@material-ui/core/Toolbar';
import Typography from '@material-ui/core/Typography';
import Button from '@material-ui/core/Button';
import { useContext } from '../utils/context';
//import Menu from '@material-ui/core/Menu';
//import MenuItem from '@material-ui/core/MenuItem';
import MenuContainer from '@material-ui/core/Menu';

import event from '../utils/event';
import Menu from './menu';

import File from './menu/file';
import Editor from './menu/editor';
import Assembler from './menu/assembler';
import Execute from './menu/execute';
import Help from './menu/help';

const useStyles = makeStyles((theme) => ({
    root: {
        flexGrow: 1,
        backgroundColor: '#191a21',
    },
    toolbar: {
        "-webkit-app-region": "drag",
        height: 36,
        minHeight: 36,
        padding: 0,
        "& div >*": {
            pointerEvents: "auto",
            "-webkit-app-region": "no-drag"
        },
        "& .MuiButton-root": {
            color: "#dadada",
            padding: "4px 8px",
            borderRadius: 0,
            textTransform: "initial",
            "&.active": {
                backgroundColor: "#535358",
            },
            "&:hover": {
                backgroundColor: "rgba(255,255,255,0.2)",
            },
        },
    },
    title: {
        color: "#fff",
        paddingLeft: 16,
        paddingRight: 8
    },
    menuRoot: {
        "& .MuiMenu-paper": {
            backgroundColor: "#535358",
            //backgroundColor: "#292b35",
            // backgroundColor: "#54555d",
            color: "#f1f1f1",
            "& .MuiMenuItem-root:hover": {
                backgroundColor: "rgba(255,255,255,0.2)"
            },
            "& .MuiListItemIcon-root":{
                color: "#f1f1f1",
                minWidth: 26
            }
        }
    },
    windowsButtons: {
        display: "flex",
        height: 36,
        "& .MuiButton-root": {
            fontFamily: 'Segoe UI',
            fontSize: 10,
            minWidth: 46,
            "&:hover": {
                backgroundColor: "rgba(255,255,255,0.3)",
            },
            "&.close:hover": {
                backgroundColor: "#E81123",
                color: "#fff"
            },
            "&.close:active": {
                backgroundColor: "#F1707A",
                color: "#fff"
            }
        }
    }
}));


const menuTop = {
    file: "Arquivo",
    editor: "Editor",
    assembler: "Montador",
    execute: "Execução",
    help: "Ajuda"
}

const Header = () => {
    const classes = useStyles();
    const [anchorEl, setAnchorEl] = useState(null);
    const [maximize, setMaximize] = useState(false);
    const [type, setType] = useState("");


    const handleToggle = (t) => (event) => {
        setType(t);
        setAnchorEl(event.currentTarget);
    };
    const handleClose = () => {
        setAnchorEl(null);
        //setType("");
    };

    const handleButton = (name) => () => {
        event("windowsAction", [name], (event, result) => {
            setMaximize(result);
        });
    }

    return (
        <div className={classes.root}>
            <AppBar position='static' style={{ backgroundColor: '#191a21' }} onDoubleClick={handleButton("restore")}>
                <Toolbar variant='dense' className={classes.toolbar}>
                    <div style={{ flexGrow: 1 }}>
                        <Typography variant="p" className={classes.title}>
                            ASMR
                        </Typography>
                        {Object.entries(menuTop).map(([key, name]) => <Button color="primary" onClick={handleToggle(key)} className={key == type ? "active" : ""}>{name}</Button>)}
                    </div>
                    <div className={classes.windowsButtons}>
                        <Button onClick={handleButton("minimize")}>&#xE921;</Button>
                        {maximize ? <Button onClick={handleButton("restore")}>&#xE923;</Button> : <Button onClick={handleButton("maximize")}>&#xE922;</Button>}
                        <Button className="close" onClick={handleButton("close")}>&#xE8BB;</Button>
                    </div>
                </Toolbar>
            </AppBar>
            <MenuContainer
                anchorEl={anchorEl}
                //                keepMounted
                className={classes.menuRoot}
                open={Boolean(anchorEl)}
                onClose={handleClose}
                style={{ marginTop: 18 }}
                anchorOrigin={{
                    horizontal: 'center'
                }}
                transformOrigin={{
                    vertical: 'bottom',
                    horizontal: 'center',
                }}>
                {type == "file" && <File onClose={handleClose} />}
                {type == "editor" && <Editor onClose={handleClose} />}
                {type == "assembler" && <Assembler onClose={handleClose} />}
                {type == "execute" && <Execute onClose={handleClose} />}
                {type == "help" && <Help onClose={handleClose} />}
            </MenuContainer>
        </div>
    );
};
/*
<Menu
    anchorEl={anchorEl}
    className={classes.menuRoot}
    type={type}
    onClose={handleClose}
    handleButton={handleButton}
/>*/

export default Header;