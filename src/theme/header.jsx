import React, { useState } from 'react';
import { makeStyles } from '@material-ui/core/styles';
import AppBar from '@material-ui/core/AppBar';
import Toolbar from '@material-ui/core/Toolbar';
import Typography from '@material-ui/core/Typography';
import Button from '@material-ui/core/Button';
import { useContext } from '../utils/context';
import Menu from '@material-ui/core/Menu';
import MenuItem from '@material-ui/core/MenuItem';

import event from '../utils/event';

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
            textTransform: "initial"
        },
    },
    title: {
        color: "#fff",
        paddingLeft: 16,
        paddingRight: 8
    },
    menuRoot: {
        "& .MuiMenu-paper": {
            //backgroundColor: "#535358",
            backgroundColor: "#292b35",
            color: "#f1f1f1",
            "& .MuiMenuItem-root:hover": {
                backgroundColor: "rgba(255,255,255,0.2)"
            }
        }
    },
    windowsButtons:{
        display:"flex",
        height: 36,
        "& .MuiButton-root": {
            fontFamily: 'Segoe UI',
            fontSize:10,
            minWidth: 46,
            "&:hover":{
                backgroundColor: "rgba(255,255,255,0.3)",
            },
            "&.close:hover":{
                backgroundColor:"#E81123",
                color:"#fff"
            },
            "&.close:active":{
                backgroundColor:"#F1707A",
                color:"#fff"
            }
        }
    }
}));



const Header = () => {
    const { memory } = useContext();
    const classes = useStyles();
    const [anchorEl, setAnchorEl] = React.useState(null);
    const [maximize,setMaximize] = useState(false);
    const handleToggle = (event) => {
        setAnchorEl(event.currentTarget);
    };
    const handleClose = () => {
        setAnchorEl(null);
    };

    const handleButton = (name) => () => {
        event("windowsAction",[name],(event,result) => {
            setMaximize(result);
        });
    }

    return (
        <div className={classes.root}>
            <AppBar position='static' style={{ backgroundColor: '#191a21' }}>
                <Toolbar variant='dense' className={classes.toolbar}>
                    <div style={{ flexGrow: 1 }}>
                        <Typography variant="p" className={classes.title}>
                            ASMR
                        </Typography>
                        <Button color="primary" onClick={handleToggle}>EU AINDA TO ARRUMANDO AQUI</Button>
                    </div>
                    <div className={classes.windowsButtons}>
                        <Button onClick={handleButton("minimize")}>&#xE921;</Button>
                        {maximize ? <Button onClick={handleButton("restore")}>&#xE923;</Button> : <Button onClick={handleButton("maximize")}>&#xE922;</Button>}
                        <Button className="close" onClick={handleButton("close")}>&#xE8BB;</Button>

                    </div>
                </Toolbar>
            </AppBar>
            <Menu
                id="simple-menu"
                anchorEl={anchorEl}
                keepMounted
                className={classes.menuRoot}
                open={Boolean(anchorEl)}
                onClose={handleClose}
                style={{ marginTop: 18 }}
                transformOrigin={{
                    vertical: 'bottom',
                    horizontal: 'center',
                }}
            >
                <MenuItem onClick={handleClose}>Profile</MenuItem>
                <MenuItem onClick={handleClose}>My account</MenuItem>
                <MenuItem onClick={handleClose}>Logout</MenuItem>
            </Menu>
        </div>
    );
};

export default Header;