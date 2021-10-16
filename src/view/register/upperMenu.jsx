import {useMemo} from 'react';
import { makeStyles } from '@material-ui/core/styles';
import Paper from '@material-ui/core/Paper';

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
    flexGrow: 1,
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
      "&>span": {
        backgroundColor: "rgba(25,86,197,0.20)",
        margin: 5,

        justifyContent: 'center',
        alignItems: 'center',
        display: 'flex',

        "&.active": {
          backgroundColor: '#1956c5'
        }
      }
    }
  }
}));

const listBits = ["OF", "SF", "ZF", "IF", "PF", "CF"]
const sr = "11100";
const pc = "1001111";
// (0, cf), (6, pf), (7, if), (8, zf), (9, sf), (12, of)

const UpperMenu = ({ register }) => {
  const classes = useStyles();

  const flags = useMemo(() => [
    register.SR.asFlags[12],
    register.SR.asFlags[9],
    register.SR.asFlags[8],
    register.SR.asFlags[7],
    register.SR.asFlags[6],
    register.SR.asFlags[0]
  ], [register]);

  console.log(register)

  return (
    <>
      <section style={{ margin: 10, display: "flex" }}>
        <Paper className={classes.root}>
          <span className={classes.name}>PC</span>
          <span className={classes.value}>{register.PC.toString("16").toUpperCase()}</span>
        </Paper>
        <Paper className={classes.root}>
          <span className={classes.name}>SR</span>
          <span className={classes.value}>{register.SR.asLiteral.toString("16").toUpperCase()}</span>
        </Paper>
        <Paper className={classes.root + " " + classes.bits}>
          <div className={classes.row}>
            {listBits.map((name, key) =>
              <div><span className={flags[key] ? "active" : ""}>{name}</span></div>
            )}
          </div>
        </Paper>
      </section>
    </>
  );
};

export default UpperMenu;
