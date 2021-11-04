import TabContainer from '@material-ui/core/Tabs';
import Tab from '@material-ui/core/Tab';
import { makeStyles } from '@material-ui/core/styles';

const useStyles = makeStyles((theme) => ({
    root: {
        backgroundColor: '#282a36',
        color: '#fff'
    }
}));

const Tabs = ({labels}) => {
    const classes = useStyles();
    
    return (
        <div className={classes.root}>
            <a>Editor</a>
            <a>Memoria</a>
        </div>

    )

}

export default Tabs;