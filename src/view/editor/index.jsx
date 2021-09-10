import React,{useState} from "react";
import AceEditor from "react-ace";
import { makeStyles } from '@material-ui/core/styles';

//assembly_x86
//teste
import "ace-builds/src-noconflict/mode-assembly_x86";
import "ace-builds/src-noconflict/theme-dracula";
import "ace-builds/src-noconflict/ext-language_tools";
import Tabs from '@material-ui/core/Tabs';
import Tab from '@material-ui/core/Tab';

const useStyles = makeStyles((theme) => ({
    root: {
        backgroundColor: '#222534 ',
        color:'#fff',
        minHeight: '2rem',
        "& 	.MuiTab-root":{
            padding: '0 12px',
            minHeight: '2rem'
        }
    }
}));


const Editor = (props) => {
    const classes = useStyles();
    const [value, setValue] = useState(2);

    function onChange(newValue) {
        console.log("change", newValue);
    }

    const handleChange = (event, newValue) => {
        setValue(newValue);
    };
    return <div id="editor" {...props}>
        <Tabs
            value={value}
            indicatorColor="primary"
            textColor="inherit"
            onChange={handleChange}
            aria-label="disabled tabs example"
            variant="scrollable"
            scrollButtons="auto"
            className={classes.root}
        >
            <Tab label="Arquivo 1"/>
            <Tab label="Arquivo 2"/>
            <Tab label="Arquivo 3" />
        </Tabs>
        <AceEditor
            mode="assembly_x86"
            theme="dracula"
            enableLiveAutocompletion="true"
            onChange={onChange}
            name="UNIQUE_ID_OF_DIV"
            style={{ width: "100%", height: "calc(100% - 2rem)", margin: "5px" }}
            editorProps={{ $blockScrolling: true }}
            setOptions={{
                showInvisibles:true,
                fontSize:20,
                enableLiveAutocompletion:true
            }}
        />
    </div>;
}

export default Editor;