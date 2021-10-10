import React from 'react';
import { makeStyles } from '@material-ui/core/styles';
import IconButton from '@material-ui/core/IconButton';
import WebAssetIcon from '@material-ui/icons/WebAsset';
import PropTypes from 'prop-types';

const useStyles = makeStyles((theme) => ({
    footer : {
        width: '100%',
        height: '1.5em',
        backgroundColor: '#6e00ff',
    },
    footerButton: {
        width: '3em',
        height: '100%',
        fontSize: '.7em',
        fontWeight: 'bold',
        color: 'white',
        marginLeft: '2em',
    }
}));

const openConsole = () => {

}

const Footer = () => {
    const classes = useStyles();
    return (
        <div className={classes.footer}>
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
        </div>
    );
};

Footer.propTypes = {};

export default Footer;