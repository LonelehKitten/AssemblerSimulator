import React, { useEffect, useState, useRef } from 'react';
import Button from '@material-ui/core/Button'
import { MenuItem } from '@material-ui/core';
import Divider from '@material-ui/core/Divider';
import { makeStyles } from '@material-ui/core/styles';
import KeyboardArrowRightIcon from '@material-ui/icons/KeyboardArrowRight';

const useStyles = makeStyles((theme) => ({
    root: {
        display: "inline-flex",
        position: "relative",
        "&.submenu":{
            width: "100%",
            "& .MuiMenuItem-root": {
                width: "100%",
                "& .MuiSvgIcon-root.submenuicon":{
                    position: "absolute",
                    right: 5,
                    zIndex: -1
                }
            },
            "& .submenu":{
                top:0,
                left: "calc(100% + 1px)",
                maxHeight: 500,
                overflow: "auto"
            }
        }
    },
    menu: {
        margin: 0,
        padding: 0,
        display: "none",
        position: "absolute",
        top: "100%",
        background: "#535358",
        zIndex: 9999,
        minWidt: 16,
        width: "max-content",
        //borderRadius: 4,
        boxShadow: "0px 5px 5px -3px rgb(0 0 0 / 20%), 0px 8px 10px 1px rgb(0 0 0 / 14%), 0px 3px 14px 2px rgb(0 0 0 / 12%)",
        "& .MuiMenuItem-root:hover": {
            backgroundColor: "rgba(255,255,255,0.2)"
        },
        "& .MuiListItemIcon-root": {
            color: "#f1f1f1",
            minWidth: 26
        },
    },
    open: {
        display: "block"
    },

}));
const Menu = ({ label, children,submenu = false }) => {
    const classes = useStyles();
    const [open, _setOpen] = useState(false);
    const ref = useRef(null);
    const openRef = useRef(open);

    const setOpen = (val) => {
        openRef.current = val;
        _setOpen(val);
    }

    const onClose = () => {
        setOpen(false);
    }

    const eventClick = (e) => {
        if (openRef.current) {
            let i = 0;
            let target = e.target;
            if(target.nodeName == "BUTTON") target = target.children[0];
            let checkSubMenu = target.parentNode;

            const isSubMenu = checkSubMenu.classList.contains("submenu");
            while(ref.current != target && i < 5){
                target = target.parentNode;
                i++;
            }
            if(ref.current !== target || checkSubMenu.nodeName != "BUTTON" && isSubMenu == false) onClose();
        }
    }

    useEffect(() => {
        window.addEventListener("click", eventClick);
        return () => window.removeEventListener("click", eventClick);
    }, []);
/*
    const childrenProps = React.Children.map((child) => {
        if(!React.isValidElement(child)) return null;

        return child;
    })*/

    return (
        <div className={[classes.root,submenu ? "submenu" : ""].join(" ")} ref={ref}>
            {submenu ? <MenuItem onClick={() => setOpen(!open)}>{label} <KeyboardArrowRightIcon className="submenuicon" /></MenuItem>: <Button onClick={() => setOpen(!open)} className={open && "active"}>{label}</Button>}
            <ul className={classes.menu + (open ? " " + classes.open : "")+ (submenu ? " submenu" : "")}>
                {children}
            </ul>
        </div>
    )
}
//{options.map((item) => item.divider ? <Divider /> : <MenuItem onClick={handleClick(item)}>{item.label}</MenuItem>)}
export default Menu;