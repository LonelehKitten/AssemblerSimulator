import React from 'react';
import { makeStyles } from '@material-ui/core/styles';
import TreeView from '@material-ui/lab/TreeView';
import ExpandMoreIcon from '@material-ui/icons/ExpandMore';
import ChevronRightIcon from '@material-ui/icons/ChevronRight';
import TreeItem from '@material-ui/lab/TreeItem';
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
//{value.children && <Item value={value.children[0]} />}

const Tree = () => {

    const {treeFiles,addFile, alertShow} = useContext();
    const openFile = (item) => () => {
        console.log("Load");    
        file.load({alertShow,addFile},item.path);
    };
    return (
        <TreeView
            defaultCollapseIcon={<ExpandMoreIcon />}
            defaultExpandIcon={<ChevronRightIcon />}
        >   
            {treeFiles != null && <Item nodeId={0} openFile={openFile} value={treeFiles} />}
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