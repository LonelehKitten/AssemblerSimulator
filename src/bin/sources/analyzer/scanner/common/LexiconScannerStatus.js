

class LexiconScannerStatus {
    constructor(accepted) {
        this.accepted = accepted;
    }
}

class SuccessStatus extends LexiconScannerStatus {
    constructor(token, tokenType, endOfLine) {
        super(true);
        this.token = token;
        this.tokenType = tokenType;
        this.endOfLine = endOfLine;
    }
}

class FailStatus extends LexiconScannerStatus {
    constructor(errorMessage) {
        super(false);
        this.errorMessage = errorMessage;
    }
}

module.exports = {
    LexiconScannerStatus,
    SuccessStatus,
    FailStatus
}