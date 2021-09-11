import React, { useState } from "react";
import { makeStyles } from '@material-ui/core/styles';
import TabContainer from '@material-ui/core/Tabs';
import Tab from '@material-ui/core/Tab';

const useStyles = makeStyles((theme) => ({
    root: {
        backgroundColor: '#222534 ',
        color: '#fff',
        minHeight: '2rem',
        "& 	.MuiTab-root": {
            padding: '0 12px',
            minHeight: '2rem'
        }
    }
}));


const Tabs = ({value, onChange,listFiles}) => {
    const classes = useStyles();
    return (
    <TabContainer
        value={value}
        indicatorColor="secondary"
        textColor="inherit"
        onChange={onChange}
        aria-label="disabled tabs example"
        variant="scrollable"
        scrollButtons="auto"
        className={classes.root}
    >
        <Tab value="" style={{display:"none"}}/>
        {Object.entries(listFiles).map(([id,file],key) => <Tab key={key} value={id} label={file.name} />)}
    </TabContainer>);
}
export default Tabs;