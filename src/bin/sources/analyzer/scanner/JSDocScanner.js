
const LexiconScanner = require('./common/LexiconScanner')

const TokenTypes = {
    METATAG: 0, // # n+2
    TYPE: 1, // # n+2
    IDENTIFIER: 2, // # n+2
    TYPE_OP: 3, // # n+1
    TYPE_ED: 4, // # n+1
    PIPE: 5, // # n+1
    GENERIC_OP: 6, // # n+1
    GENERIC_ED: 7, // # n+1
    //LINK: 8,
}

const TokenSet = {
    M_PARAM:    ["@param",  TokenTypes.METATAG],
    M_RETURN:   ["@return", TokenTypes.METATAG],

    TYPE_OP:    ["{",       TokenTypes.TYPE_OP],
    TYPE_ED:    ["}",       TokenTypes.TYPE_ED],
    GENERIC_OP: ["<",       TokenTypes.GENERIC_OP],
    GENERIC_ED: [">",       TokenTypes.GENERIC_ED],
    PIPE:       ["|",       TokenTypes.PIPE],

    T_NUMBER:   ["Number",  TokenTypes.TYPE],
    T_STRING:   ["String",  TokenTypes.TYPE],
    T_BOOLEAN:  ["Boolean", TokenTypes.TYPE],
    T_ARRAY:    ["Array",   TokenTypes.TYPE],
    T_OBJECT:   ["Object",  TokenTypes.TYPE],
}

const Tokens = LexiconScanner.createDictionary(TokenSet);

/*
const Tokens = {
    "@param": TokenTypes.METATAG,
    "@return": TokenTypes.METATAG,
    "{": TokenTypes.TYPE_OP,
    "}": TokenTypes.TYPE_ED,
    "<": TokenTypes.GENERIC_OP,
    ">": TokenTypes.GENERIC_ED,
    "|": TokenTypes.PIPE,
    "Number": TokenTypes.TYPE,
    "String": TokenTypes.TYPE,
    "Boolean": TokenTypes.TYPE,
    "Object": TokenTypes.TYPE,
    "Array": TokenTypes.TYPE
}



q0Begin_metatag 



q0Begin_metatag -> q1_metatag
q1_metatag 		-> q1_metatag | q2End_metatag
q2End_metatag 	-> #

*/

class JSDocScanner extends LexiconScanner {

    /** 
     * @description
     * Essa função é antiga pra um c$%&* e não tenho ideia pra que serve
     * mas quando tiro quebra o programa.
     * 
     * @alert Não apagar
     * 
     * @param {Number} a - é pra isso
     * @param {String} b - é pra aquilo
     * 
     * @return {Boolean} - não faço ideia, mas é melhor não mexer
     */
    
    constructor() {
        super();
        this.tokenTypes = TokenTypes;
        this.tokens = Tokens;
        this.initialStates = [
            this.q0Begin_metatag,
            this.q0Begin_type,
            this.q0Begin_name,
            this.qBegin_s,
            this.qBegin_s,
            this.qBegin_s,
            this.qBegin_s,
            this.qBegin_s
        ];
    }
    
    // <metatag>
    q0Begin_metatag = () => {
        return this.q([{condition: this.isSpecial(), state: this.q1_metatag}]);
    }

    q1_metatag = () => {
        return this.q([{condition: this.isAlphaLower(), state: this.q1_metatag}], {state: this.q2End_metatag});
    }

    q2End_metatag = () => {
        return this.qEnd({conditions: [this.tokenTypes.METATAG], deterministic: false, tokenType: this.tokenTypes.METATAG});
    }
    // </metatag>
    
    // <type>
    q0Begin_type = () => {
        return this.q([{condition: this.isAlphaUpper(), state: this.q1_type}]);
    }

    q1_type = () => {
        return this.q([{condition: this.isAlphaNumeric(), state: this.q1_type}], {state: this.q2End_type});
    }

    q2End_type = () => {
        return this.qEnd({conditions: [this.tokenTypes.TYPE], deterministic: false, tokenType: this.tokenTypes.TYPE});
    }
    // </type>

    
    // <pipe, typeOp, typeEd, genericOp, genericEd>
    qBegin_s = () => {
        return this.q([{condition: this.isSpecial(), state: this.qEnd_s}]);
    }
    
    qEnd_s = () => {
        return this.qEnd({conditions: [
            this.tokenTypes.PIPE,
            this.tokenTypes.GENERIC_OP,
            this.tokenTypes.GENERIC_ED,
            this.tokenTypes.TYPE_OP,
            this.tokenTypes.TYPE_ED
        ], stateName: "qEnd_s"})
    }
    // </ pipe, typeOp, typeEd, genericOp, genericEd>

    
    // <identifier>
    q0Begin_name = () => {
        return this.q([{condition: this.isAlpha() || this.currentChar == '_', state: this.q1_name}]);
    }

    q1_name = () => {
        return this.q([{condition: this.isAlphaNumeric() || this.currentChar === '_', state: this.q1_name}], {state: this.q2End_name});
    }

    // obrigatorio ser identificador
    q2End_name = () => {
        return this.qEnd({conditions: !this.isUnknownToken(), defaultAction: {
            tokenType: this.tokenTypes.IDENTIFIER
        }})
    }
    // </identifier>
}

JSDocScanner.TokenTypes = TokenTypes;
JSDocScanner.TokenSet = TokenSet;
JSDocScanner.Tokens = Tokens;

module.exports = JSDocScanner;