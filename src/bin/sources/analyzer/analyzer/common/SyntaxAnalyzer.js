const Stack = require('../../utils/data/Stack');
const LexiconScanner = require("../../scanner/common/LexiconScanner");

class SyntaxAnalyzer {

    /**
     * linha a ser lida
     */
    line = ""
    tokenData = null
    eof = false;
    scanner = null;
    stack = null;

    /** 
     * Array de tokens reconhecidos
    */
    row = null;

    error = false;

    constructor (scanner) {
        if(!(scanner instanceof LexiconScanner)) 
            throw new Error("scanner must be of type LexiconScanner");
        this.scanner = scanner;
        this.stack = new Stack();
    }

    /**
     * seta a proxima linha
     */
    set = (line, last) => {
        this.line = line;
        this.last = last;
    }

    state = () => null

    /**
     * inicia a análise
     */
    init = () => {
        this.log("init");
        this.row = [];
        this.error = false;
        if(this.scanner !== null) {
            this.scanner.setLine(this.line);
            while(true) {
                if(this.state() && !this.error) {
                    return true;
                }
                if(this.error) return false;
            }
        }
        return false;
    }

    /**
     * prediz o conteudo da linha baseado nos primeiros caracteres
     * definindo qual token pedir primeiro e qual analise lexica fazer
     */
    /** @abstract */ predict = () => null

    /** @abstract */ check = (recognizer) => null

    /** @abstract */ getRow = (recognizer) => null

    trim = () => {
        this.line = this.line.trim() + '\n';
    }


    // ===========

    log = (msg) => {
        console.log("------------------------------")
        console.log("%c DEBUG: "+msg, "color: yellow")
        console.log("------------------------------")
    }

    error = (msg) => {
        console.log("%c ERROR: "+msg, "background: red")
        let column = "";
        for(let i = 0; i < this.documentationScanner.lineIndex; i++)
            column += " "
        column += "^";
        console.log(this.documentationScanner.line);
        console.log(column)
    }

}

module.exports = SyntaxAnalyzer;