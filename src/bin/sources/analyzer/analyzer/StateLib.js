class StateLib {

    tokenData = null;

    constructor(analyzer) {
        this.analyzer = analyzer;
        this.scanner = analyzer.scanner;
        this.nextToken = analyzer.scanner.nextToken;
        this.tokenTypes = analyzer.scanner.tokenTypes;
        this.tokens = analyzer.scanner.tokens;

        this.stack = analyzer.stack;
    }

    defaultState = ({tokenType, state=null, token='', stackExpectedValue='', push=null, customFlag=true, load=true, id=false, callback=null}) => {
        this.analyzer.error = false;
        if(load) this.tokenData = this.nextToken(tokenType);
        if(
            (id || this.tokens[this.tokenData.token] === tokenType) &&
            (!id || this.tokens[this.tokenData.token] === undefined) &&
            (stackExpectedValue === '' || this.stack.getContent() === stackExpectedValue) &&
            (token === '' || this.tokenData.token === token) &&
            customFlag
        ) {

            this.log(this.tokenData.token);

            if(stackExpectedValue !== '') this.stack.pop();
            if(push !== null) this.stack.push(push);
            if(callback !== null) callback(this);
            if(state !== null) this.analyzer.state = state;
            this.analyzer.row.push({...this.tokenData});
            return false;
        }
        this.analyzer.error = true;
        
        return true;
    }

    log = (msg) => {
        console.log("%c DEBUG: "+msg, "color: yellow")
    }

}

class TypeReadingState {

    onSuccess = () => null

    constructor(stateLib, onSuccess) {
        this.stateLib = stateLib;
        this.onSuccess = onSuccess;
    }

    wasArray = false;

    qStart = () => {
        return this.stateLib.defaultState({tokenType: this.stateLib.tokenTypes.TYPE_OP, state: this.q1, push: 'type'});
     }

     q1 = () => {
        return this.stateLib.defaultState({tokenType: this.stateLib.tokenTypes.TYPE, state: this.qEnd, 
            callback: () => {this.wasArray = (this.stateLib.tokenData.token === 'Array')}
        });
     }
 
     qEnd = () => {
 
         let r = this.stateLib.defaultState({tokenType: this.stateLib.tokenTypes.TYPE_ED, state: this.onSuccess(), stackExpectedValue: 'type'});
 
         if(r && this.wasArray) {
             r = this.stateLib.defaultState({tokenType: this.stateLib.tokenTypes.GENERIC_OP, state: this.q1, push: 'generic', load: false,
                 callback: () => {this.wasArray = false}
             });
         }
         
         if(r) r = this.stateLib.defaultState({tokenType: this.stateLib.tokenTypes.GENERIC_ED, state: this.qEnd, stackExpectedValue: 'generic', load: false});
 
         if(r) {
             r = this.stateLib.defaultState({tokenType: this.stateLib.tokenTypes.PIPE, state: this.q1, load: false,
                 callback: () => {this.wasArray = false}
             });
         }
 
         return r;
     }

}

class ParamListReadingState {

    onSuccess = () => null

    constructor(stateLib, onSuccess) {
        this.stateLib = stateLib;
        this.onSuccess = onSuccess;
    }

    qStart = () => {
        console.log('ParamListReadingState#qStart')
        let r = this.stateLib.defaultState({tokenType: this.stateLib.tokenTypes.IDENTIFIER, state: this.q1, id: true})
        if(r) {
            this.stateLib.analyzer.error = false;
            this.stateLib.analyzer.state = this.onSuccess()
            //r = this.onSuccess()
        }
        return false;
    }
    // fazer um debug apartir da linha 112
    q1 = () => {
        console.log('ParamListReadingState#q1')
        let r = this.stateLib.defaultState({tokenType: this.stateLib.tokenTypes.SEPARATOR, state: this.q2})
        if(r) {
            //console.log(this.onSuccess())
            this.stateLib.analyzer.error = false;
            this.stateLib.analyzer.state = this.onSuccess()
            
        }
        return false;
    }

    q2 = () => {
        console.log('ParamListReadingState#q2')
        let r = this.stateLib.defaultState({tokenType: this.stateLib.tokenTypes.IDENTIFIER, state: this.q1, id: true})
        if(r) {
            this.stateLib.analyzer.error = false;
            this.stateLib.analyzer.state = this.onSuccess()
            //r = this.onSuccess()
        }
        return false;
    }

}

// a={a:[{s:2}]}
// v
// [i, b, {d, x}]
teste = () => {

}

StateLib.TypeReadingState = TypeReadingState;
StateLib.ParamListReadingState = ParamListReadingState;
module.exports = StateLib;
