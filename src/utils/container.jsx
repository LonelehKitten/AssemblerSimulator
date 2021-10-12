import React, {useState, useEffect} from 'react';
import PropTypes from 'prop-types';
import { makeStyles } from '@material-ui/core/styles';
import Dragger from './dragger';

const useStyles = makeStyles((theme) => ({
    container: {
        display: 'flex',
        flexWrap: 'wrap',
    },
    horizontal: {
        width: '100%',
    },
    vertical: {
        height: '100%',
    },
}));

const Container = ({orientation='vertical', draggable=false, onResize=null, value, className='', style={}, children}) => {
    const classes = useStyles();
    const orientationClass = orientation === 'vertical' ? classes.vertical : classes.horizontal;

    let fix = orientation === 'vertical' ? 
        {width: value + 'px' } :
        {height: value + 'px' };
    
    if(!draggable) {
        fix = orientation === 'vertical' ? 
            {width: `calc(100% - ${value}px)` } :
            {height: `calc(100% - ${value}px + 20px - 1.5em)` };
    }
    
    fix = {...fix, ...style}
    
    return (
        <div className={className + ' ' + classes.container + ' ' + orientationClass} style={fix}>
            {draggable && <Dragger orientation={orientation} onMouse={onResize} />}
            <div 
            style={{
                width: draggable && orientation === 'vertical' ? 'calc(100% - 5px) ' : '100%',
                height: draggable && orientation === 'horizontal' ? 'calc(100% - 5px) ' : '100%',
                display: 'flex',
                flexWrap: 'wrap',
            }}
            >
                {children}
            </div>
        </div>
    );
};

Container.propTypes = {};

export default Container;