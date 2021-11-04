import * as file from './file.js';
import * as electron from './electron.js';
import { useContext } from './context.js';
const isEmpty = (value) => value == '' || value == null;

export {
    isEmpty,
    file,
    electron,
    useContext
};