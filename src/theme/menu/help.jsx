import { useEffect, useState } from 'react';
import MenuItem from '@material-ui/core/MenuItem';
import ListItemIcon from '@material-ui/core/ListItemIcon';
import CheckIcon from '@material-ui/icons/Check';
import Divider from '@material-ui/core/Divider';
import Dialog from '@material-ui/core/Dialog';
import DialogTitle from '@material-ui/core/DialogTitle';
import DialogContent from '@material-ui/core/DialogContent';
import Avatar from '@material-ui/core/Avatar';
import { makeStyles } from '@material-ui/core/styles';
import GitHubIcon from '@material-ui/icons/GitHub';
import Menu from '../../components/menu';
import Grid from '@material-ui/core/Grid';
import Tooltip from '@material-ui/core/Tooltip';

import users from './users.json';
import event from '../../utils/event';

const useStyles = makeStyles({
    root: {
        "& .MuiDialogContent-root": {
            "&::-webkit-scrollbar": {
                width: "0.4em",
                height: "0.4em"
            },
            "&::-webkit-scrollbar-track": {
                "-webkit-box-shadow": "inset 0 0 6px rgba(0, 0, 0, 0.02)"
            },
            "&::-webkit-scrollbar-thumb": {
                outline: "1px solid transparent",
                backgroundColor: "#191b24"
            },
            "&::-webkit-scrollbar-corner": {
                background: "#282a36"
            },
        },
        "& .MuiPaper-root": {
            maxWidth: 617,
            backgroundColor: "#21222c",
            color: "#f1f1f1",
            "&>.MuiDialogTitle-root":{
                padding: "8px 24px",
                "& .MuiTypography-h6":{
                    fontSize: "1.5rem"
                }
            },
        },
        "& .user": {
            display: "flex",
            "& .MuiAvatar-root": {
                width: "4em",
                height: "4em",
                marginRight: 10,
                display: "inline-flex",
                cursor: "pointer"
            },
            "& h3": {
                margin: "6px 0"
            },
            "& a": {
                color: "#f1f1f1",
                margin: "0 5px"
            }
        }
    }
});

const listLinks = {
    github: {
        name: 'GitHUB',
        icon: <GitHubIcon />
    }
};

const openLink = (link) => () => {
    window.electron.remote.shell.openExternal(link);
}

const User = ({ item }) => {
    return (
        <Grid xs={6} item className="user">
            <Avatar alt="Remy Sharp" src={item.image} />
            <div>
                <h3>{item.name}</h3>
                {Object.entries(item.links).map(([type, link]) => {
                    const Info = listLinks[type];
                    return <Tooltip title={Info.name}><a onClick={openLink(link)}>{Info.icon}</a></Tooltip>;
                })}
            </div>
        </Grid>
    );
}

const Help = () => {
    const classes = useStyles();
    const [open, setOpen] = useState(false);
    const [version,setVersion] = useState("");

    useEffect(() => {  
        event("getVersion",[],(e,value) => {
            setVersion(value);
        })
    })

    return (
        <>
            <Menu label="Ajuda">
                <MenuItem>Ajuda</MenuItem>
                <MenuItem onClick={() => setOpen(true)}>Colaboradores</MenuItem>
                <MenuItem onClick={openLink("https://github.com/LonelehKitten/AssemblerSimulator")}>Versão {version}</MenuItem>
            </Menu>
            <Dialog onClose={() => setOpen(false)} className={classes.root} open={open}>
                <DialogTitle style={{fontSize:22}}>Colaboradores</DialogTitle>
                <DialogContent>
                    {users.map((item) => (
                        <>
                            <DialogTitle style={{paddingLeft:0}}>{item.name}</DialogTitle>
                            <Grid container spacing={5}>
                                {item.users.map((user) => <User item={user} />)}
                            </Grid>
                        </>
                    ))}
                </DialogContent>
            </Dialog>
        </>
    );
}

export default Help;