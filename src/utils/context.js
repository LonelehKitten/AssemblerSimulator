import React,{useContext as uC} from 'react';

export const Context = React.createContext('chat');
export const useContext = () => uC(Context);