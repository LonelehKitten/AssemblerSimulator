import { useState } from "react";


const Console = (props) => {
    const [history, setHistory] = useState([]);

    const handleSubmit = (e) => {
        if(e.keyCode == 13){
            const value = e.target.value;
            if(value == "") return;
            setHistory((oldValue) => [...oldValue,value]);
            e.target.value = "";
        }
    }

    return <div id="console" {...props}>
        {history.map((item) => <div>{"> "+item}</div>)}
<br />
        <input onKeyDown={handleSubmit} placeholder="Digite um comando..." style={{width:"100%",backgroundColor:"transparent",border:0,color:"#fff",fontSize:20,outline:0}} />
    </div>;
}

export default Console;