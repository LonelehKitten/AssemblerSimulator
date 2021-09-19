import { makeStyles } from '@material-ui/core/styles';

const UpperMenu = (props) => {
  const useStyles = makeStyles((theme) => ({
    root: {
      display: 'flex',
      width: `${props.width}`,
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
  }));

  const classes = useStyles();

  return (
    <>
      <section className={classes.root}>
        <div className={classes.firstSector}>texto</div>
        <div className={classes.sector}>texto</div>
      </section>
    </>
  );
};

export default UpperMenu;
