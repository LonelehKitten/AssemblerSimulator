import { makeStyles } from '@material-ui/core/styles';

const useStyles = makeStyles((theme) => ({
  root: {
    display: 'flex',
    flexWrap: 'wrap',
    marginTop: '0',
    height: '88%', //100% //'calc(70vh - 3rem)',
  },
  table: {
    alignContent: 'center',
    alignItems: 'center',
    justifyContent: 'center',
    color: 'inherit',
    backgroundColor: '#313241',
    overflow: 'auto',
    overflowY: 'scroll',
    '& .MuiTableCell-root': {
      borderColor: '#999',
      color: '#fff',
    },
  },
  register: {
    width: '100%',
    height: '5em',
    margin: '.25em 1em',
    backgroundColor: '#313241',
    borderRadius: '.75em',
    borderTop: '.2em solid #6e00ff',
    boxShadow: '0 .2em .5em #0005',
    display: 'flex',
    justifyContent: 'center',
    alignItems: 'center',
    fontWeight: 'bold',
    textAlign: 'center',
    '& .name': {
      display: 'flex',
      width: '25%',
      justifyContent: 'center',
      alignItems: 'center',
    },
    '& .bits': {
      display: 'flex',
      flexWrap: 'wrap',
      width: '50%',
      justifyContent: 'center',
      alignItems: 'center',
      '& > div': {
        width: '10.5%',
        height: '50%',
        margin: '1%',
        '&:nth-child(1)': {
          borderRadius: '.75em 0 0 0',
        },
        '&:nth-child(8)': {
          borderRadius: '0 .75em 0 0',
        },
        '&:nth-child(9)': {
          borderRadius: '0 0 0 .75em',
        },
        '&:nth-child(16)': {
          borderRadius: '0 0 .75em 0',
        },
      },
    },
    '& .value': {
      width: '25%',
      display: 'flex',
      justifyContent: 'center',
      alignItems: 'center',
    },
  },
}));

const Bit = ({ value }) => (
  <div
    style={{
      fontSize: 10,
      backgroundColor: value == 1 ? '#1b6cff' : '#1b6cff00',
      border: '1px solid #1b6cff',
      width: '.7em',
      height: '.7em',
      display: 'inline-block',
      borderRadius: '50%',
      margin: '0 .1em',
    }}
  ></div>
);

const Register = ({ name, value }) => {
  const classes = useStyles();
  const bits = parseInt(value).toString('2').padStart(16, 0);
  return (
    <div className={classes.register}>
      <div className='name'>{name}</div>
      <div className='bits'>
        {[...bits].map((bit, key) => (
          <div
            key={key}
            style={{ backgroundColor: bit === '1' ? '#1b6cff' : '#1b6cff44' }}
          >
            &nbsp;
          </div>
        ))}
      </div>
      <div className='value'>{parseInt(value).toString(16).padStart(4, 0)}</div>
    </div>
  );
};

const LowerMenu = ({ register }) => {
  const classes = useStyles();
  return (
    <section className={classes.root}>
      <Register name='AX' value={register.AX} />
      <Register name='DX' value={register.DX} />
      <Register name='SI' value={register.SI} />
      <Register name='DS' value={register.DS} />
      <Register name='CS' value={register.CS} />
      <Register name='SS' value={register.SS} />
      <Register name='SP' value={register.SP} />
    </section>
  );
};

export default LowerMenu;
