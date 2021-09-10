import { makeStyles } from '@material-ui/core/styles';

const useStyles = makeStyles(theme => ({
    dragger: {
        padding: "4px 0 0",
        borderTop: "1px solid #363a52",
        position: "absolute",
        zIndex: 100,
        backgroundColor: "#363a52"
    },
    horizontal:{
        cursor: "n-resize",
        left: 0,
        right: 0,
        height: "3px",
    },
    vertical:{
        cursor: "ew-resize",
        top: 0,
        left: 0,
        bottom: 0,
        width: "5px",
    }
  }));
  
  const Dragger = ({onMouse,orientation = "vertical"}) => {
    const classes = useStyles();
    const orientationClasse = orientation == "horizontal" ? classes.horizontal : classes.vertical;

    const handleMouseDown = e => {
        document.addEventListener("mouseup", handleMouseUp, true);
        document.addEventListener("mousemove", onMouse, true);
    };

    const handleMouseUp = () => {
        document.removeEventListener("mouseup", handleMouseUp, true);
        document.removeEventListener("mousemove", onMouse, true);
    };

    return <div onMouseDown={handleMouseDown} className={classes.dragger+" "+orientationClasse} />;
}

export default Dragger;