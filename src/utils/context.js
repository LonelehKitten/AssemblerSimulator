import React, { useContext as uC } from 'react';

export const Context = React.createContext('assembler');
export const useContext = () => uC(Context);
