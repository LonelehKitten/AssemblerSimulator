import { makeStyles } from '@material-ui/core/styles';
import UpperMenu from './upperMenu';
import LowerMenu from './lowerMenu';
import { ClassNames } from '@emotion/react';
//import Grid from '@material-ui/core/Grid';

// Esse não tem, mas ussa esse aqui: https://material-ui.com/pt/components/tables/

const useStyles = makeStyles((theme) => ({
  root: {
    backgroundColor: '#21222c',
  },
}));
/*

<UpperMenu width={props.width}></UpperMenu>
*/

const Register = (props) => {
  const classes = useStyles();

  return (
    <div id='register' className={classes.root}>
      <UpperMenu />
      <LowerMenu />
    </div>
  );
};

export default Register;
