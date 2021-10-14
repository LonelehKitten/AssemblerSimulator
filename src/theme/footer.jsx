import React from 'react';
import { makeStyles } from '@material-ui/core/styles';
import IconButton from '@material-ui/core/IconButton';
import WebAssetIcon from '@material-ui/icons/WebAsset';
import { useContext } from '../utils/context';
import PropTypes from 'prop-types';

const useStyles = makeStyles((theme) => ({
    footer : {
        width: '100%',
        height: '1.5em',
        backgroundColor: '#6e00ff',
    },
    footerButton: {
        width: '7em',
        height: '100%',
        fontSize: '.7em',
        fontWeight: 'bold',
        color: 'white',
        backgroundColor: '#fff0',
        display: 'flex',
        alignItems: 'center',
        justifyContent: 'flex-start',
        transition: 'all .1s',
        cursor: 'pointer',
        userSelect: 'none',
        '&:hover': {
            backgroundColor: '#fff8'
        }
    }
}));

const Footer = () => {
    const { consoleOpen, setConsoleOpen } = useContext();
    const toggleConsole = () => {
        setConsoleOpen(!consoleOpen)
    }

    const classes = useStyles();
    return (
        <div className={classes.footer}>
           <div className={classes.footerButton} onClick={toggleConsole}>
                <WebAssetIcon style={{fontSize: '1.5em', margin: '0 .25em'}}  /> 
                Console
           </div>
        </div>
    );
};

/*
 <IconButton
            edge='start'
            className={classes.footerButton}
            color='inherit'
            aria-label='menu'
            onClick={openConsole}
          >
            <WebAssetIcon style={{fontSize: '1.5em', marginRight: '.25em'}} /> 
            Console
          </IconButton>

*/

Footer.propTypes = {};

export default Footer;