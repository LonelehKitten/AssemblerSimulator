import React from 'react';
import { makeStyles } from '@material-ui/core/styles';
import TreeView from '@material-ui/lab/TreeView';
import ExpandMoreIcon from '@material-ui/icons/ExpandMore';
import ChevronRightIcon from '@material-ui/icons/ChevronRight';
import TreeItem from '@material-ui/lab/TreeItem';
import Button from '@material-ui/core/Button'
import Typography from '@material-ui/core/Typography';
import { useContext } from '../../utils';
import {file} from '../../utils';


const Item = ({value,nodeId,openFile}) => {
    //  console.log(value,value.children[0]);
    return (
        <TreeItem nodeId={nodeId} label={value.name} onClick={value.type == "file" && openFile(value)}>
            {value.children && value.children.map((i,key) => <Item nodeId={nodeId + 5 * key+1} openFile={openFile} value={i} />)}
        </TreeItem>
    );
    
}

const useStyles = makeStyles({
    root:{
        margin: "50px auto",
        display: "block",
        textAlign: "center",
        "& button":{
            marginTop: "1em",
            backgroundColor: "#282a36",
            color: "#f1f1f1",
            borderRadius:0
        }
    }
});
//{value.children && <Item value={value.children[0]} />}

const Tree = () => {
    const classes = useStyles();
    const {treeFiles,addFile, alertShow,setTreeFiles} = useContext();

    const openFile = (item) => () => {
        file.load({alertShow,addFile},item.path);
    };
    const OpenDirectory = () => {
        file.getTree({setTreeFiles,alertShow});
    }

    return (
        <TreeView
            defaultCollapseIcon={<ExpandMoreIcon />}
            defaultExpandIcon={<ChevronRightIcon />}
        >   
            {treeFiles != null ? <Item nodeId={0} openFile={openFile} value={treeFiles} /> : (
                <div className={classes.root}>
                    <Typography>Abra um diretório para visualiar a árvore de arquivos.</Typography>
                    <Button variant="contained" onClick={OpenDirectory}>Abrir Diretório</Button>
                </div>
            )}
        </TreeView>
    )
}

export default Tree;
/*
            <TreeItem nodeId="5" label="Documents">
                <TreeItem nodeId="10" label="OSS" />
                <TreeItem nodeId="6" label="Material-UI">
                    <TreeItem nodeId="7" label="src">
                        <TreeItem nodeId="8" label="index.js" />
                        <TreeItem nodeId="9" label="tree-view.js" />
                    </TreeItem>
                </TreeItem>
            </TreeItem>*/