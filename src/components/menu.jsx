import MenuContainer from '@material-ui/core/Menu';
import MenuItem from '@material-ui/core/MenuItem';
import Divider from '@material-ui/core/Divider';

const Menu = ({onClose,options,...props}) => {

    const handleClick = (item) => () => {
        item.onClick();
        onClose();
    }

    return (
    <MenuContainer onClose={onClose} {...props}>
        {options.map((item) => item.divider ? <Divider /> : <MenuItem onClick={handleClick(item)}>{item.label}</MenuItem>)}
    </MenuContainer>);
}
export default Menu;