import {useState} from 'react';
import Snackbar from '@material-ui/core/Snackbar';
import MuiAlert from '@material-ui/lab/Alert';

const AlertModal = ({onClose, color, message,...props}) => {

    const handleClose = (event, reason) => {
        if (reason === 'clickaway') {
            return;
        }
        onClose(null);
    };

    return (
        <Snackbar open={true} autoHideDuration={message.time || 6000} onClose={handleClose}>
            <MuiAlert onClose={handleClose} severity={message.color} elevation={6} variant="filled" {...props}>
                {message.text}
            </MuiAlert>
        </Snackbar>
    );
}

const Alert = ({message,...props}) => {
    return message && <AlertModal message={message} {...props} />
}
export default Alert;
