import React, { useState } from 'react';
import { makeStyles } from '@material-ui/core/styles';
import IconButton from '@material-ui/core/IconButton';
import WebAssetIcon from '@material-ui/icons/WebAsset';
import { useContext } from '../utils/context';
import PropTypes from 'prop-types';

import Slider from '@material-ui/core/Slider';

const useStyles = makeStyles((theme) => ({
    footer: {
        display: 'flex',
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
        display: 'inline-flex',
        alignItems: 'center',
        justifyContent: 'flex-start',
        transition: 'all .1s',
        cursor: 'pointer',
        userSelect: 'none',
        '&:hover': {
            backgroundColor: '#fff8'
        }
    },
    slider: {
        display: 'inline-flex',
        maxHeight: '1.5em',
        overflow: 'hidden',
        /*
        '& .MuiSlider-valueLabel': {
            top: 'inherit',
            bottom: -51,
            '&>span': {
                transform: 'rotate(135deg)',
                '& >span': {
                    transform: 'rotate(-135deg)',
                    color: '#333',
                },
            },
        },*/
    },
    counterClock:{
        padding: '4px 0',
        fontWeight: 'bold',
        fontSize: 14,
        marginLeft: -13
    }
}));

const Footer = () => {
    const { consoleOpen, setConsoleOpen } = useContext();
    const [clock, setClock] = useState(16);


    const toggleConsole = () => {
        setConsoleOpen(!consoleOpen)
    }

    const classes = useStyles();
    return (
        <div className={classes.footer}>
            <div className={classes.footerButton} onClick={toggleConsole}>
                <WebAssetIcon style={{ fontSize: '1.5em', margin: '0 .25em' }} />
                Console
            </div>
            <div className={classes.slider}>
                <Slider
                    defaultValue={16}
                   // valueLabelDisplay='auto'
                    step={1}
                    marks
                    min={1}
                    max={20}
                    onChange={(e, value) => setClock(value)}
                    value={clock}
                    style={{
                        color: '#fff',
                        width: 200,
                        marginLeft: '1em',
                        marginRight: '2em',
                    }}
                />
                <span className={classes.counterClock}>{clock} Clock</span>
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