import { makeStyles } from '@material-ui/core/styles';

const UpperMenu = (props) => {
    const useStyles = makeStyles((theme) => ({
        root: {
            display: "flex",
            width: `${props.width}`,
            height: "80px",
            margin: "5px",
            padding: "10px",
        },
        sector: {
            display: "block",
            width: "calc(50vw - 10px)",
            height: "5rem",
            backgroundColor: theme.palette.primary.main,
            padding: "1rem"
        },
        firstSector: {
            marginRight: "10px",
            display: "block",
            width: "50vw",
            height: "5rem",
            backgroundColor: theme.palette.primary.main,
            padding: "1rem"
        }
    }));
    
    const classes = useStyles();

    return (
        <> 
            <section className={classes.root}>
                <div className={classes.firstSector}>
                    texto
                </div>
                <div className={classes.sector}>
                    texto
                </div>
            </section>
        </>
    )
}

export default UpperMenu;