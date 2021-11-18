import MenuItem from '@material-ui/core/MenuItem';

import { useContext, electron } from '../../utils';
import Menu from '../../components/menu';


const requests = {
    requestExpandMacros: 'Expandir Macros',
    requestTest: 'Executar Teste',
    requestBuildAndRun: 'Construir e Executar',
    requestAssembleAndRun: 'Montar e Executar',
    requestAssembleAndRunBySteps: 'Montar e Executar Por Passos',
    requestRun: 'Enviar bytecode e executar',
    requestRunBySteps: 'Enviar bytecode e executar Por Passos',
    requestNextStep: 'Próximo Passo',
    requestKillProcess: 'Encerrar Processo',
    simulate: 'Simular',
};


const Execute = () => {
    const { memory, currentFile, setPlaying, setByStep, treeFiles } = useContext();

    const handleClick = (type) => () => {
        electron.request.play({ type, memory, currentFile, setPlaying, setByStep, treeFiles });
    }

    return (
        <Menu label="Execução">
            {Object.entries(requests).map(([key, name]) => (
                <MenuItem onClick={handleClick(key)}>{name} ({key})</MenuItem>
            ))}
        </Menu>
    );
}

export default Execute;