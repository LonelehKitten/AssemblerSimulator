import { useState } from "react";

import { makeStyles } from '@material-ui/core/styles';

const Console = (props) => {

    const useStyles = makeStyles((theme) => ({
        textField: {
            fontFamily: "VT323",
            marginBottom: "5px",
            backgroundColor:"transparent",
            border: `3px inset ${theme.palette.primary.main}`,
            padding: "10px",
            color:"#fff",
            backgroundColor: "#282a36",
            fontSize:24,
            outline:0,
            borderRadius: "3px",
        },
        inputedTexts: {
            padding: "0.2em", 
            backgroundColor: "#646b91", 
            margin: "0.2em",
            borderRadius: "20px",
            width: "max-content"
        }
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
        <div id="console" {...props} >
            {history.map((item) => <div className={classes.inputedTexts}>
                {"> "+item}
                </div>)
            }
            <br />
            <input onKeyDown={handleSubmit} placeholder="Digite um comando . . ." className={classes.textField} />
        </div>
    )
}

export default Console;