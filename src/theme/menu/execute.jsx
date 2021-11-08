import MenuItem from '@material-ui/core/MenuItem';

import { useContext, electron } from '../../utils'; 


const requests = {
    requestExpandMacros: 'Expandir Macros',
    requestTest: 'Executar Teste',
    requestAssembleAndRun: 'Construir e Executar',
    requestAssembleAndRunBySteps: 'Construir e Executar Por Passos',
    requestRun: 'Enviar bytecode e executar',
    requestRunBySteps: 'Enviar bytecode e executar Por Passos',
    requestNextStep: 'Próximo Passo',
    requestKillProcess: 'Encerrar Processo',
    simulate: 'Simular',
};


const Execute = ({ onClose }) => {
    const {memory,currentFile,setPlaying,setByStep} = useContext();

    const handleClick = (type) => () => {
        electron.request.play({ type, memory, currentFile, setPlaying, setByStep });
        onClose();
    }

    return (
        <>
            {Object.entries(requests).map(([key, name]) => (
                <MenuItem onClick={handleClick(key)}>{name} ({key})</MenuItem>
            ))}
        </>
    );
}

export default Execute;