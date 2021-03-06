import event from './event';
import { getTree } from './file';
// Para chamar o module do C++

const getBytecodeFromEditor = (currentFile) => {
    return currentFile?.code
        .replace(/[^0-9a-fA-F]/g, '')
        .match(/.{2}/g)
        .map((current) => parseInt('0x'.concat(current)))
        .filter((current) => !isNaN(current));
};
const isEmpty = (value) => value == '' || value == null;

const getAllFiles = (tree,array = []) => {
    if(typeof tree.children == "undefined"){
        array.push(tree);
    }else{
        tree.children.map((i) => {
             getAllFiles(i,array);
        });
    }
    return array;
}
const play = async ({ type, memory, currentFile, setPlaying, setByStep, treeFiles, modeAssembler }) => {
    let memoryToBytes = [];
    for (let i = 0; i < memory.length; i++) {
        let string = String(memory[i]).padStart(4, 0);
        memoryToBytes.push(parseInt(string.substr(0, 2), 16));
        memoryToBytes.push(parseInt(string.substr(2, 2), 16));
    }
    // play_expandMacros
    if (!isEmpty(currentFile?.code) || type == 'simulate') {
        // Habilita ou não o "Playing"
        if (type == 'simulate' || type == 'requestEndTest' || type == 'requestTest' || type == 'requestKillProcess') setPlaying(false);
        else setPlaying(true);

        const params = [];

        // Adicionar as instruções
        if (['requestRun', 'requestRunBySteps'].includes(type)) {
            let arr = getBytecodeFromEditor(currentFile)
            params.push(arr);
        }
        if (['requestBuildAndRun'].includes(type) || modeAssembler == 1) {
            let files = treeFiles;
            if (files == null) files = await getTree();
            files = getAllFiles(files).map(({ path }) => path);
            console.log(files);
            params.push(files);
        }
        if (['requestExpandMacros', 'requestAssembleAndRun', 'requestAssembleAndRunBySteps'].includes(type) && modeAssembler == 0) {
            params.push(currentFile?.code);
        }
        if (['requestAssembleAndRun', 'requestAssembleAndRunBySteps', 'requestRun', 'requestRunBySteps'].includes(type)) {
            params.push(memoryToBytes);
        }
        if (['requestRunBySteps', 'requestAssembleAndRunBySteps', 'requestNextStep'].includes(type)) {
            console.log("Header|BySteyp", type, true);
            setByStep(true);
        } else {
            console.log("Header|BySteyp", type, false);
            setByStep(false);
        }
        event('play', [type, params]);
    }
}

const request = {
    play
}

export { request };