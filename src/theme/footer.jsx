import React from 'react';
import { makeStyles } from '@material-ui/core/styles';
import PropTypes from 'prop-types';

const useStyles = makeStyles((theme) => ({
    footer : {
        width: '100%',
        height: '1.5em',
        backgroundColor: '#6e00ff'
    }
}));

const Footer = () => {
    const classes = useStyles();
    return (
        <div className={classes.footer}>
            
        </div>
    );
};

Footer.propTypes = {};

export default Footer;