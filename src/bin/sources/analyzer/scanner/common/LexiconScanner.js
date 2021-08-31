const {SuccessStatus, FailStatus} = require('./LexiconScannerStatus');

/** @abstract */ class LexiconScanner {

    /**
     * tipos de tokens que são reconhecidos
     */
    tokenTypes = null;

    /**
     * Dicionario de tokens reconhecidos
     * chave: o token
     * valor: tipo do token
     */
    tokens = null;

    /**
     * Linha a ser lida
     */
    line = '';

    /**
     * O token que está sendo reconhecido de line
     */
    token = '';

    /**
     * Caractere atualmente lido de line
     */
    currentChar = '';

    /**
     * Posição do cabeçote na linha
     */
    lineIndex = 0;

    /**
     * Flag que indica quando o cabeçote chegou no final da linha
     */
    endOfLine = false;

    /**
     * Dicionario de estados iniciais de cada autômato que reconhece cada tipo de token
     * chave: tipo do token
     * valor: referencia do estado inicial respectivo
     */
    initialStates = null

    /**
     * Informações do token reconhecido requisitadas pelo analisador sintatico
     */
    tokenData = null;

    /**
     * Flag que indica quando a palavra é aceita pelo autômato
     */
    accepted = false;

    /**
     * Flag que indica quando um automato não reconhece o token lido.
     * Indica um erro léxico
     */
    error = false;

    /**
     * Mensagem de erro com as informações de onde ocorreu
     */
    errormsg = ""

    lastTokenBeginPosition = null;

    lastTokenEndPosition = null;

    stack = null;

    /**
     * Seta a próxima linha a ser lida e reseta as configurações do analisador léxico
     * @param {String} line - linha a ser lida
     */
    setLine = (line) => {
        this.currentChar = line.charAt(0);
        this.line = line;
        this.endOfLine = false;
        this.lineIndex = 0;
        this.lastTokenBeginPosition = 0;
        this.lastTokenEndPosition = 0;
    }

    /**
     * desfaz a ultima leitura
     * @alert apenas 1 vez
     */
    undo = () => {
        this.endOfLine = false;
        this.lastTokenEndPosition = this.lastTokenBeginPosition;
    }

    /**
     * Lê o próximo token de line usando o estado inicial referente ao tipo de token informado
     * 
     * @param {Number} tokenType - O tipo do próximo token que deve ser reconhecido
     * @returns {Object} - Retorna o token reconhecido, seu tipo, o indicador de final de linha e qual analisador lexico foi utilisado
     */
    nextToken = (tokenType) => {

        this.start(tokenType)

        while(true) {

            //console.log(this.token);
            
            this.log()

            if(this.state() && !this.error){
                //if(this.token.charAt(this.token.length -1) === ' ')
                  //  console.log("tem espaço");

                return {...this.tokenData}
            }

            if(this.error) {
                console.log("ERROR");
                return new FailStatus(`Unknown token: ${this.token}`);
            }

            //if(!this.accepted) 
            this.nextChar();

            //if(this.accepted) this.fix();
            
        }
    }

    log = () => {
        let column = "";
        for(let i = 0; i < this.lineIndex; i++)
            column += " "
        column += "^";
        console.log(this.line);
        console.log(column)
    }

    // @param{Number}altura

    start = (tokenType) => {
        this.token = "";
        this.snap();
        this.lastTokenBeginPosition = this.lineIndex;
        this.state = this.initialStates[tokenType];
    }

    accept = (deterministic=true) => {
        this.lastTokenEndPosition = this.lineIndex;
        if(!deterministic) {
            this.lastTokenEndPosition--;
        }
    }

    /**
     * le os espaços e os ignora levando o cabeçote até antes do inicio do próximo token
     */
    snap = () => {
        this.currentChar = this.line.charAt(this.lastTokenEndPosition);
        if(this.isNull()){
            while(this.line.charAt(this.lastTokenEndPosition) === " ") {
                this.lastTokenEndPosition++;
            }
        }
        this.lineIndex = this.lastTokenEndPosition;
        this.currentChar = this.line.charAt(this.lineIndex);
    }

    /**
     * Lê o próximo caractere de line
     */
    nextChar = () => {
        this.lineIndex++;
        this.currentChar = this.line.charAt(this.lineIndex)
    }

    previousChar = () => {
        this.lineIndex--;
        this.currentChar = this.line.charAt(this.lineIndex)
    }

    setErrorMsg = (f) => {
        this.errormsg  = "[Erro]\n"
        this.errormsg += "state:           "+f+"\n"
        this.errormsg += "current char:    "+this.currentChar+"\n"
        this.errormsg += "token:           "+this.token+"\n"
    }

    setSuccessMessage = (tokenType=this.tokens[this.token]) => {
        this.tokenData = new SuccessStatus(this.token, tokenType, this.endOfLine);
    }

    checkEndOfLine = (nonDeterministic) => {
        if(nonDeterministic) this.endOfLine = (this.line.charAt(this.lineIndex-1) === '\n');
        else this.endOfLine = (this.line.charAt(this.lineIndex) === '\n');
    }

    /**
     * Ponteiro para os estados dos autômatos
     * @returns - true se o token for aceito, false caso contrário
     */
    /** @abstract */ state = () => null;
    
    /** @deprecated @abstract */ tokenize = () => null


    // ======== automatos padrao =========

    /*
    
    predicates is Array<Predicates>

    Predicate: {
        condition: condição de aceitação,
        state: estado de transição,
        pop?: [null] valor esperado na pilha, se condição aceita, a ultima entrada é desempilhada
        push?: [null] valor a ser empilhado se condição for aceita
        emptyStack?: [false] só aceita se a pilha estiver vazia
    }
    
    */

    q = (predicates, defaultPredicate=null) => {

        for (const p in predicates) {
            if (
                predicates[p].condition &&
                (!predicates[p].pop || this.isStackValue(predicates[p].pop)) &&
                (!predicates[p].emptyStack || this.stack.isEmpty())
            ) {
                if(predicates[p].push) this.stack.push(predicates[p].push)
                if(predicates[p].pop) this.stack.pop()
                this.token += this.currentChar;
                this.state = predicates[p].state;
                return false;
            }
        }

        if(defaultPredicate) {
            if(defaultPredicate.move) this.token += this.currentChar;
            this.state = defaultPredicate.state;
        }
        else this.error = true;

        return false;

    }

    qEnd = ({conditions, defaultAction=null, deterministic=true, tokenType=null, stateName='[it wasn\'t given]'}) => {
        
        this.checkEndOfLine(deterministic);
        
        //let condition = conditions;
        let condition = (this.tokens[this.token] === conditions)
        
        if(conditions instanceof Array) {
            condition = false;
            for (const c in conditions) {
                if(this.tokens[this.token] === conditions[c]) {
                    condition = true;
                    break;
                }
            }
        }

        if (condition) {
            console.log("qEnd Success")
            this.accept(deterministic)
            if(tokenType) this.setSuccessMessage(tokenType);
            else this.setSuccessMessage();
            return true
        }

        if(defaultAction) {
            this.accept(defaultAction.deterministic)
            if(defaultAction.tokenType) this.setSuccessMessage(defaultAction.tokenType);
            else this.setSuccessMessage();
            return true
        }

        this.error = true
        this.setErrorMsg(stateName);
        
        return true
    }

    // ========

    static createDictionary = (tokenSet) => {
        let dictionary = {};
        Object.keys(tokenSet).forEach(key => {
            dictionary[tokenSet[key][0]] = tokenSet[key][1]
        })

        console.log(dictionary)
        return dictionary;
    }

    // ========

    isStackValue = (value) => {
        return this.stack.getContent() === value;
    }

    // verifica se o token é reconhecido pelo dicionário, caso contrário pode ser um identificador ou indicar erro
    isUnknownToken = () => this.tokens[this.token] === undefined

    // comparadores

    is = (char) => this.currentChar === char

    // conjunto α U Α (alfa minúsculo união alfa maiúsculo)
    isAlpha = () => {
        return (
            (this.currentChar >= 'a' && this.currentChar <= 'z') || 
            (this.currentChar >= 'A' && this.currentChar <= 'Z')
        )
    }

    isAlphaUpper = () => {
        return (
            (this.currentChar >= 'A' && this.currentChar <= 'Z')
        )
    }

    isAlphaLower = () => {
        return (
            (this.currentChar >= 'a' && this.currentChar <= 'z') 
        )
    }
    
    // conjunto Γ (gama)
    isNumeric = () => this.currentChar >= '0' && this.currentChar <= '9'

    // conjunto Γ (gama) ou α U Α (alfa minúsculo união alfa maiúsculo)
    isAlphaNumeric = () =>  this.isAlpha() || this.isNumeric()
    
    // valor ε (epsilon)
    isNull = () => this.currentChar === ' ' || this.currentChar === '\0'

    // valor Φ (fi)
    isLineBreak = () => this.currentChar === '\n'

    // conjunto Σ (sigma)
    isSpecial = () => !this.isAlpha() && !this.isNumeric() && !this.isLineBreak() && !this.isNull()

}

module.exports = LexiconScanner;