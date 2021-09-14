import AceEditor from "react-ace";
import "ace-builds/src-noconflict/mode-assembly_x86";
import "ace-builds/src-noconflict/theme-dracula";
import "ace-builds/src-noconflict/ext-language_tools";

import { useContext } from "../../utils/context";
import { useEffect, useState } from "react";


const {ipcRenderer} = window.electron;

const Ace = ({ onChange }) => {
    const { currentFile, setCode } = useContext();
    const [teste,setTeste] = useState(true);

    const handleChange = (value) => {
        setCode(value);
    }

    const handleSave = (editor) => {
        setTeste(!teste);
        console.log(currentFile);
        ipcRenderer.send("invoke_save_file",JSON.stringify(currentFile));
        ipcRenderer.once("save_file", (e,success) => {
            if(success){
                alertShow("success","Arquivo Salvo");
            }else{
                alertShow("danger","Erro ao salvar o arquivo");
            }
        });
    }

    useEffect(() => {
        console.log("cccaaa");
    },[teste])

    return (
        <AceEditor
            commands={[{
                name: 'save',
                bindKey: {win: "Ctrl-S", "mac": "Cmd-S"},
                exec: (e) => {
                    console.log("aaa");
                    handleSave(e);
                }

            }]}
            readOnly={currentFile === null}
            mode="assembly_x86"
            theme="dracula"
            placeholder={currentFile === null ? "Crie um arquivo . . ." : "" }
            onChange={handleChange}
            name="UNIQUE_ID_OF_DIV"
            value={currentFile?.code}
            style={{ width: "100%", height: "calc(100% - 2rem)" }}
            editorProps={{ $blockScrolling: true }}
            setOptions={{
                showInvisibles: true,
                fontSize: 20,
                enableLiveAutocompletion: true
            }}
        />);
}

export default Ace;