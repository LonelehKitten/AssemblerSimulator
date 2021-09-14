import { useState } from "react";

import { makeStyles } from '@material-ui/core/styles';

const Console = (props) => {

    const useStyles = makeStyles((theme) => ({
        textField: {
            margin: "5px",
            width: `{ 100vw - ${props.width} }`,
            backgroundColor:"transparent",
            borderTop: `3px solid ${theme.palette.primary.main}`,
            borderLeft: "0",
            borderRight: "0",
            borderBottom: "0",
            padding: "10px",
            color:"#fff",
            fontSize:20,
            outline:0
        },
    }));

    const [history, setHistory] = useState([]);
    
    const classes = useStyles();

    const handleSubmit = (e) => {
        if(e.keyCode == 13){
            const value = e.target.value;
            if(value == "") return;
            setHistory((oldValue) => [...oldValue,value]);
            e.target.value = "";
        }
    }
 
    return (
        <div id="console" {...props}>
            {history.map((item) => <div>{"> "+item}</div>)}
            <br />
            <input onKeyDown={handleSubmit} placeholder="Digite um comando . . ." className={classes.textField} />
        </div>
    )
}

export default Console;