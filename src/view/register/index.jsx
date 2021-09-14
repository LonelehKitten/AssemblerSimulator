import { makeStyles } from '@material-ui/core/styles';
import UpperMenu from './upperMenu';
import LowerMenu from './lowerMenu';
//import Grid from '@material-ui/core/Grid';

// Esse não tem, mas ussa esse aqui: https://material-ui.com/pt/components/tables/

// const useStyles = makeStyles((theme) => ({
//     root: {
//       flexGrow: 1,
//     },
// }));
/*

*/

const Register = (props) => {
    return (
        <div id="register"> 
            <UpperMenu width={props.width} ></UpperMenu>
            <LowerMenu width={props.width} height={props.height}></LowerMenu>            
        </div>
    );
}

export default Register;