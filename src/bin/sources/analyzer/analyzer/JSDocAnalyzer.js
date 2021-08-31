const SyntaxAnalyzer = require('./common/SyntaxAnalyzer')
const JSDocScanner = require("../scanner/JSDocScanner");
const StateLib = require('../utils/automaton/StateLib');
const Extensions = require('../core/Extensions')

const TokenTypes = JSDocScanner.TokenTypes;
const TokenSet = JSDocScanner.TokenSet;

class JSDocAnalyzer extends SyntaxAnalyzer {

    constructor() {
        super(new JSDocScanner());
    }

    /**
     * prediz o conteudo da linha baseado nos primeiros caracteres
     * definindo qual token pedir primeiro e qual analise lexica fazer
     */
    predict = (char) => {
        switch (char) {
            case '@':  
                this.state = this.qBegin_metatag;
                return;
        }
    }

    x = '';

    qBegin_metatag = () => {
        //return this.q({tokenType: this.tokenTypes.METATAG, state: this.q1, token: '@param'})
        let q1 = new StateLib.TypeReadingState(new StateLib(this), () => {
            switch (this.x) {
                case TokenSet.M_PARAM[0]:
                    return this.q4;
                case TokenSet.M_RETURN[0]:
                    return this.qEnd;
                default:
                    return () => true;
            }
        }).qStart
        return new StateLib(this).defaultState({tokenType: TokenTypes.METATAG, state: q1, callback: (it) => {this.x = it.tokenData.token}})
    }

    q4 = () => {
        return new StateLib(this).defaultState({tokenType: TokenTypes.IDENTIFIER, state: this.qEnd, id: true})
    }

    qEnd = () => {
        console.log('Sucesso!')
        return true;
    }

    // ===========

    check = (recognizer) => {

        this.trim();

        let docIndicator = true;
        
        for(let i = 0; i < this.line.length; i++) {

            if(this.line.charAt(i) === " ") continue;

            if(this.line.charAt(i) === '*' && this.line.charAt(i+1) === '/') {
                console.log("[IDENTIFICOU FIM DE JAVADOC]")
                recognizer.enableDocMode(false);
                return false;
            }

            if(this.line.charAt(i) === "*" && docIndicator) {
                console.log("[IDENTIFICOU INDICADOR DE DOCUMENTACAO]")
                this.line = this.line.substr(i+1);
                i--;
                docIndicator = false;
                continue;
            }

            if(this.line.charAt(i) !== ' ' && this.line.charAt(i) !== '\n') {
                console.log("[IDENTIFICOU TEXTO RECONHECIVEL]")
                return true;
            }    
            
        }

        return false;
    }

    getRow = () => {
        const row = {extension: Extensions.JS, doc: true}

        

        return row
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

module.exports = JSDocAnalyzer;