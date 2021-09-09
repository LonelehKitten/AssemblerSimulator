import React from "react";
import AceEditor from "react-ace";
//assembly_x86
import "ace-builds/src-noconflict/mode-assembly_x86";
import "ace-builds/src-noconflict/theme-dracula";
import Tabs from '@material-ui/core/Tabs';
import Tab from '@material-ui/core/Tab';
const Editor = (props) => {
    function onChange(newValue) {
        console.log("change", newValue);
    }
    const [value, setValue] = React.useState(2);

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
        >
            <Tab label="Arquivo 1" />
            <Tab label="Arquivo 2" />
            <Tab label="Arquivo 3" />
        </Tabs>
        <AceEditor
            mode="assembly_x86"
            theme="dracula"
            onChange={onChange}
            name="UNIQUE_ID_OF_DIV"
            style={{ width: "100%", height: "100%   " }}
            editorProps={{ $blockScrolling: true }}
        />
    </div>;
}

export default Editor;