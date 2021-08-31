const SyntaxAnalyzer = require('./common/SyntaxAnalyzer')
const JavascriptScanner = require("../scanner/JavascriptScanner");
const StateLib = require('../utils/automaton/StateLib');

const TokenTypes = JavascriptScanner.TokenTypes;
const TokenSet = JavascriptScanner.TokenSet;
const Tokens = JavascriptScanner.Tokens;

class JavascriptAnalyzer extends SyntaxAnalyzer {


    // provavelmente temporario
    constructor() {
        super(new JavascriptScanner());
    }

    /**
     * prediz o conteudo da linha baseado nos primeiros caracteres
     * definindo qual token pedir primeiro e qual analise lexica fazer
     */
    predict = (char) => {
        console.log("predict#char    "+char)
        switch (char) {
            case 'l':
            case 'c':
            case 'f':
                console.log("ok")
                this.state = this.qBegin_declaration
                return;
        }
        
    }

    qBegin_declaration = () => {
        console.log('qBegin_declaration')
        return new StateLib(this).defaultState({
            tokenType: TokenTypes.DECLARATION, 
            callback: (stateLib) => {
                console.log("defaultState#callback    "+stateLib.tokenData.token)
                if(stateLib.tokenData.token === TokenSet.D_FUNCTION[0]) {
                    this.state = this.q1b
                    return
                }
                this.state = this.q1a
            }
        })
    }

    q1a = () => {
        
    }

    q1b = () => {
        return new StateLib(this).defaultState({tokenType: TokenTypes.IDENTIFIER, state: this.q1b_2, id: true})
    }

    q1b_2 = () => {
        return new StateLib(this).defaultState({tokenType: TokenTypes.PARAM_OP, state: this.q1b_2_3})
    }

    q1b_2_3 = () => {
        return new StateLib.ParamListReadingState(new StateLib(this), () => this.q1b_2_3_4).qStart()
    }

    q1b_2_3_4 = () => {
        console.log("q1b_2_3_4")
        this.scanner.undo()
        return new StateLib(this).defaultState({tokenType: TokenTypes.PARAM_ED, state: this.qFunction})
    }

    qFunction = () => {
        console.log('sucesso');
        return true;
    }

    check = (recognizer) => {
        this.trim();
        // Exemplo
        // i = 5, j = 8, posMax = 14, length = 15
        //     i   j
        // 0000/*00*/00000
        // line.substr(0, i) + line.substr(j+2, line.length-j-2)
        // '0000' + '00000' (  15 - 8 - 2 = 5 )

        let isValid = true;
        first:
        for(let i = 0; i < this.line.length; i++) {

            if(this.line.charAt(i) === ' ') continue;
            
            // limpa comentario de multiplas linhas quando em mais de uma linha
            if(this.multilineComment) {
                if(this.line.charAt(i) === '*' && this.line.charAt(i+1) === '/') {
                    this.line = this.line.substr(i+2);
                    this.multilineComment = false;
                }
                continue;
            }

            if(
                this.line.charAt(i) === '/' &&
                this.line.charAt(i+1) === '*' &&
                this.line.charAt(i+2) === '*'
            ) {
                console.log("[IDENTIFICOU O INICIO DE JAVADOC]")
                recognizer.enableDocMode(true);
                return false;
            }

            // extrai comentarios de multiplas linhas
            if(this.line.charAt(i) === '/' && this.line.charAt(i+1) === '*') {
                for(let j = i; j < this.line.length; j++) {
                    if(this.line.charAt(j) === '*' && this.line.charAt(j+1) === '/') {
                        this.line = this.line.substr(0, i) + this.line.substr(j+2, this.line.length-j-2)
                        i--;
                        continue first;
                    }
                }
                this.multilineComment = true;
                break;
            }
            // verifica se possui comentario de linha unica
            if(this.line.charAt(i) === '/' && this.line.charAt(i+1) === '/') {
                this.line = this.line.substr(0, i);
                break;
            }
            // verifica se possui conteudo processavel pelo analisador
            if(this.line.charAt() !== ' ' && this.line.charAt() !== '\n') 
                isValid = true;
        }
        return isValid;
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

module.exports = JavascriptAnalyzer;