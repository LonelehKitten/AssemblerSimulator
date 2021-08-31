
const LexiconScanner = require('./common/LexiconScanner')
const Stack = require('../utils/data/Stack');

const TokenTypes = {
    DECLARATION: 0,
    IDENTIFIER: 1,
    ASSIGNMENT:  2,
    PARAM_OP: 3,
    PARAM_ED: 4,
    SEPARATOR: 5,
    SCOPE_OP: 6,
    SCOPE_ED: 7,
    DEFAULT_VALUE: 8,
}

const TokenSet = {
    D_VAR:      ["var", TokenTypes.DECLARATION],
    D_LET:      ["let", TokenTypes.DECLARATION],
    D_CONST:    ["const", TokenTypes.DECLARATION],
    D_FUNCTION: ["function", TokenTypes.DECLARATION],
    ASSIGNMENT: ["=", TokenTypes.ASSIGNMENT],
    PARAM_OP:   ["(", TokenTypes.PARAM_OP],
    PARAM_ED:   [")", TokenTypes.PARAM_ED],
    SEPARATOR:  [",", TokenTypes.SEPARATOR],
    SCOPE_OP:   ["{", TokenTypes.SCOPE_OP],
    SCOPE_ED:   ["}", TokenTypes.SCOPE_ED],
}

const Tokens = LexiconScanner.createDictionary(TokenSet)

class JavascriptScanner extends LexiconScanner {
    
    constructor() {
        super();
        this.tokenTypes = TokenTypes;
        this.tokens = Tokens;
        this.initialStates = [
            this.qBegin_a,
            this.q0Begin_b,
            this.qBegin_c,
            this.qBegin_c,
            this.qBegin_c,
            this.qBegin_c,
            this.qBegin_c,
            this.qBegin_c,
            this.qBegin_defaultValue
        ]
        this.stack = new Stack();
    }

    // <declaration, function>
    qBegin_a = () => {
        console.log("bbbbbbbbbb")
        // if(this.isAlpha()) {
        //     this.token += this.currentChar;
        //     return false
        // }
        // this.state = this.qEnd_a
        // return false;
        
        return this.q([{condition: this.isAlphaLower(), state: this.qBegin_a}], {state: this.qEnd_a});
    }

    
    qEnd_a = () => {

        // if(
        //     this.tokenTypes[this.token] == this.tokenTypes.DECLARATION ||
        //     this.tokenTypes[this.token] == this.tokenTypes.FUNCTION
        //     ){
        //     this.tokenData = {
        //         token: this.token,
        //         tokenType: this.tokens[this.token],
        //         endOfLine: this.endOfLine
        //     }
        //     return true;
        // }
        // this.error = true;
        // return true;
        return this.qEnd({conditions: this.tokenTypes.DECLARATION, deterministic: false, tokenType: this.tokenTypes.DECLARATION});
    }
    // </declaration, function>

    //<id>
    q0Begin_b = () => {
        console.log("aaaaaaaaaaaaaaaaa")
        // if(this.isAlpha()) {
        //     this.token += this.currentChar
        //     this.state = this.q1_b;
        //     return false;
        // }
        // this.error = true;
        // return false;
        return this.q([{condition: this.isAlpha() || this.currentChar === '_', state: this.q1_b}]);
    }

    q1_b = () => {

        // if(this.isAlphaNumeric() || this.currentChar == '_') {
        //     this.token += this.currentChar;
        //     return false;
        // }
        // this.state = this.q2End_b
        // return false;
        return this.q([{condition: this.isAlphaNumeric() || this.currentChar === '_', state: this.q1_b}], {state: this.q2End_b});
    }

    q2End_b = () => {

        // if(!this.isUnknownToken()) {
        //     this.tokenData = {
        //         token: this.token,
        //         tokenType: this.tokens[this.token],
        //         endOfLine: this.endOfLine
        //     }
        //     return true;
        // }

        // this.tokenData = {
        //     token: this.token,
        //     tokenType: this.tokenTypes.ID,
        //     endOfLine: this.endOfLine
        // }

        // return true;

        return this.qEnd({conditions: !this.isUnknownToken(), deterministic: false, defaultAction: {
            tokenType: this.tokenTypes.IDENTIFIER,
            deterministic: false
        }})
    }
    //</id>

    // <assignment, paramOp, paramEd>
    qBegin_c = () => {

        // if (this.isSpecial()) {
        //     this.token += this.currentChar
        //     this.state = this.qEnd_c
        //     return false;
        // }
        // this.error = true
        // return false;

        return this.q([{condition: this.isSpecial(), state: this.qEnd_c}])
        
    }

    
    qEnd_c = () => {

        // if(
        //     this.tokenTypes[this.token] == this.tokenTypes.ASSIGNMENT ||
        //     this.tokenTypes[this.token] == this.tokenTypes.PARAM_OP ||
        //     this.tokenTypes[this.token] == this.tokenTypes.PARAM_ED ||
        //     this.tokenTypes[this.token] == this.tokenTypes.SEPARATOR 
        //     ) {
        //     this.tokenData = {
        //         token: this.token,
        //         tokenType: this.tokens[this.token],
        //         endOfLine: this.endOfLine
        //     }
        //     return true;
        // }
        // this.error = true;
        // return true;

        return this.qEnd({conditions: [
            this.tokenTypes.ASSIGNMENT,
            this.tokenTypes.PARAM_OP,
            this.tokenTypes.PARAM_ED,
            this.tokenTypes.SEPARATOR,
            this.tokenTypes.SCOPE_OP,
            this.tokenTypes.SCOPE_ED,
        ]})
    }
    // </assignment. paramOp, paramEd>

    qBegin_defaultValue = () => {
        return this.q([{
            condition: !(this.is(',') && this.is(')')), 
            state: this.q1_defaultValue,
            push: this.push_defaultValue(), //só quando { ou [, cc. nulo
            pop: this.pop_defaultValue(),   //só quando } ou ], cc. nulo
        }])
    }
    q1_defaultValue = () => {
        return this.q(
        [{
            condition: this.is(',') || this.is(')'),
            state: this.qEnd_defaultValue,
            stackEmpty: true
        },
        {
            condition: true, 
            state: this.qBegin_defaultValue,
            push: this.push_defaultValue(), //só quando { ou [, cc. nulo
            pop: this.pop_defaultValue(),   //só quando } ou ], cc. nulo
        }])
    }

    push_defaultValue = () => {
        if(this.is('{') && this.is('[') || this.is('(')) {
            if(this.is('{')) return 'o'
            if(this.is('[')) return 'a'
            if(this.is('(')) return 'e'
        }
        return null
    }

    pop_defaultValue = () => {
        if(this.is('}') || this.is(']') || this.is(')')) {
            if(this.is('}')) return 'o'
            if(this.is(']')) return 'a'
            if(this.is(')')) return 'e'
        }
        return null
    }

    qEnd_defaultValue = () => {
        return this.qEnd({condition: this.tokenTypes.DEFAULT_VALUE})
    }

}

JavascriptScanner.TokenTypes = TokenTypes;
JavascriptScanner.TokenSet = TokenSet;
JavascriptScanner.Tokens = Tokens;

module.exports = JavascriptScanner;