import AceEditor from "react-ace";
import "ace-builds/src-noconflict/mode-assembly_x86";
import "ace-builds/src-noconflict/theme-dracula";
import "ace-builds/src-noconflict/ext-language_tools";

import { useContext } from "../../utils/context";

const Ace = ({ onChange }) => {
    const { currentFile, setFile } = useContext();

    const handleChange = (value) => {
        setFile(value);
    }

    return (
        <AceEditor
            readOnly={currentFile === null}
            mode="assembly_x86"
            theme="dracula"
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