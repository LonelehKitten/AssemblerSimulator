import { makeStyles } from '@material-ui/core/styles';
import Paper from '@material-ui/core/Paper';

/*
 root: {
    display: 'flex',
    width: '100%',
    height: '7rem',
    margin: '1rem',
    padding: '10px',
  },
  sector: {
    display: 'block',
    width: 'calc(50vw - 10px)',
    height: '5rem',
    backgroundColor: '#313241',
    padding: '1rem',
    borderRadius: '3px',
  },
  firstSector: {
    marginRight: '10px',
    display: 'block',
    width: '48vw',
    height: '5rem',
    backgroundColor: '#313241',
    padding: '1rem',
    borderRadius: '3px',
  },
*/
const useStyles = makeStyles((theme, props) => ({
  root: {
    position: "relative",
    backgroundColor: "#313241",
    color: "#fff",
    display: "block",
    margin: "0 5px",
    flexGrow: 9,
    display: 'flex',
    justifyContent: 'center',
    alignItems: 'center',
    "&:first-child": {
      marginLeft: 0
    },
    "&:last-child": {
      marginLeft: 0
    }
  },
  name: {
    position: "absolute",
    top: 2,
    left: 2,
    color: "#dedede"
  },
  value: {
    color: "#fff",
    fontSize: 52,
    textAlign: 'center',
    display: 'flex',
  },
  bits: {
    flexGrow:1,
    backgroundColor: "initial",
    display: "block"
  },
  row: {
    display: 'flex',
    flexDirection: 'row',
    flexWrap: 'wrap',
    width: '100%',
    "& div": {
      display: 'flex',
      flexDirection: 'column',
      flexBasis: '50%',
      flex: 1,
      "&>span":{
        backgroundColor:"rgba(25,86,197,0.20)",
        margin:5,
        
        justifyContent: 'center',
        alignItems: 'center',
        display: 'flex',

        "&.active":{
          backgroundColor: '#1956c5'
        }
      }
    }
  }
}));

const listBits = ["OF", "SF", "ZF", "IF", "PF", "CF"]
const sr = "11100";
const pc = "1001111";
const UpperMenu = (props) => {
  const classes = useStyles();

  return (
    <>
      <section style={{ margin: 10, display: "flex" }}>
        <Paper className={classes.root}>
          <span className={classes.name}>PC</span>
          <span className={classes.value}>{parseInt(pc,2).toString("16").toUpperCase()}</span>
        </Paper>
        <Paper className={classes.root}>
          <span className={classes.name}>SR</span>
          <span className={classes.value}>{parseInt(sr,2).toString("16").toUpperCase()}</span>
        </Paper>
        <Paper className={classes.root + " " + classes.bits}>
          <div className={classes.row}>
            {listBits.map((name,key) =>
              <div><span className={key % 3 == 1 ? "active" : ""}>{name}</span></div>
            )}
          </div>
        </Paper>
      </section>
    </>
  );
};

export default UpperMenu;
