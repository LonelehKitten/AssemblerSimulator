import React, { useState } from 'react';
import { useContext } from '../../utils/context';

import Ace from './ace';
import Tabs from './tabs';
import Memory from './memory';

const Editor = (props) => {
  const { listFiles, changeFile } = useContext();
  const [value, setValue] = useState('');

  const handleChange = (event, newValue) => {
    setValue(newValue);
    changeFile(newValue);
  };

  return (
    <div id='editor' {...props}>
      <Tabs value={value} onChange={handleChange} listFiles={listFiles} />
      <Ace onChange={() => console.log('aa')} />
    </div>
  );
};

<Memory />
export default Editor;
