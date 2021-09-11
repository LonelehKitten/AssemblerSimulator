const Queue = require('../utils/data/Queue');

const {Analyzers} = require('./Config');

class RecognitionManager{

    /**
     * linha a ser lida
     */
    line = ""
    
    currentAnalyzer = null;

    pageRows = null

    docMode = false;

    /**
     * seta a proxima linha
     */
    set = (line, last) => {
        this.getAnalyzer().set(line, last);
    }

    newPage = () => {
        this.pageRows = new Queue();
    }

    /**
     * prediz o conteudo da linha baseado nos primeiros caracteres
     * definindo qual token pedir primeiro e qual analise lexica fazer
     */
    predict = () => {
        this.log("predict");

        let char = "";
        for (let i = 0; i < this.getAnalyzer().line.length; i++) {
            char = this.getAnalyzer().line.charAt(i);
            if(char === " ") continue;
            this.getAnalyzer().predict(char);
            break;
        }

    }


    /*
    

    pilha de macro: macro1 macro2
    macro1: (p1, p2) add sub mul
    macro2: (q1, q2) add sub

    scope_counter 0

    ======================================

    add AX, AD

    begin macro1 p1, p2

        add AX, DX
        sub AX, p1


        begin macro2 q1, q2

            add AX, DX
            sub AX, p1

        end macro2


        mul

    end macro1

    macro1 0 1

    macro2

    ============== RESULTADO =============

    add AX, AD
    add AX, DX
    sub AX, 1

    */




    /**
     * inicia a análise
     */
    init = () => {
        this.log("init");

        this.predict();

        if(this.getAnalyzer().init()) {
            //console.table(this.getAnalyzer().getRow())
            //this.pageRows.push(this.getAnalyzer().getRow())
            console.table(this.getAnalyzer().row)
            this.pageRows.push(this.getAnalyzer().row)
        }

    }

    

    // ===========
    
    isValid = () => {

        console.log("VALIDATING...")

        if(this.getAnalyzer().check(this)) {
            
            return true;
        }

        return false;

    }

    getAnalyzer = () => {
        if(this.docMode) 
            return Analyzers[this.currentAnalyzer].doc;

        return Analyzers[this.currentAnalyzer].code
    }

    setCurrentAnalyzer = (currentAnalyzer) => {
        this.currentAnalyzer = currentAnalyzer;
    }

    isCurrentAnalyzerEquals = (value) => this.currentAnalyzer === value;

    enableDocMode = (docMode) => {
        this.docMode = docMode;
    }

    getPage = () => this.getAnalyzer().pageRows

    // ===========

    log = (msg) => {
        console.log("------------------------------")
        console.log("%c DEBUG: "+msg, "color: yellow")
        console.log("------------------------------")
    }

    error = (msg) => {
        console.log("%c ERROR: "+msg, "background: red")
        let column = "";
        for(let i = 0; i < this.getAnalyzer().lineIndex; i++)
            column += " "
        column += "^";
        console.log(this.getAnalyzer().line);
        console.log(column)
    }

}

module.exports = RecognitionManager;