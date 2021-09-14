import { makeStyles } from '@material-ui/core/styles';
import UpperMenu from './upperMenu/upperMenu';
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
        <> 
            <UpperMenu width={props.width} ></UpperMenu>            
        </>
    );
}

export default Register;