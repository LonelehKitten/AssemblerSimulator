#include "SyntaxAutomatons.h"
#include "SyntaxAnalyzer.h"

namespace SyntaxAutomatons {

    Transition::Transition(AutomatonPattern automatonPattern, TokenTypes tokenType) :
        automatonPattern(automatonPattern),
        tokenType(tokenType),
        state(nullptr),
        token(""),
        pop(""),
        push(""),
        customFlag(true),
        load(true),
        id(false),
        callback(nullptr)
    {}

    AutomatonPattern Transition::getAutomatonPattern() {
        return automatonPattern;
    }

    void Transition::setAutomatonPattern(AutomatonPattern automatonPattern) {
        this->automatonPattern = automatonPattern;
    }

    TokenTypes Transition::getTokenType() {
        return tokenType;
    }

    void Transition::setTokenType(TokenTypes tokenType) {
        this->tokenType = tokenType;
    }

    const std::function<bool(SyntaxAnalyzer *)> Transition::getState() {
        return state;
    }

    void Transition::setState(const std::function<bool(SyntaxAnalyzer *)> state) {
        this->state = state;
    }

    std::string Transition::getToken() {
        return token;
    }

    void Transition::setToken(std::string token) {
        this->token = token;
    }

    std::string Transition::getPop() {
        return pop;
    }

    void Transition::setPop(std::string pop) {
        this->pop = pop;
    }

    std::string Transition::getPush() {
        return push;
    }

    void Transition::setPush(std::string push) {
        this->push = push;
    }

    bool Transition::getCustomFlag() {
        return customFlag;
    }

    void Transition::setCustomFlag(bool customFlag) {
        this->customFlag = customFlag;
    }

    bool Transition::getLoad() {
        return load;
    }

    void Transition::setLoad(bool load) {
        this->load = load;
    }

    bool Transition::isId() {
        return id;
    }

    void Transition::setId(bool id) {
        this->id = id;
    }

    const std::function<void(SyntaxAnalyzer *)> Transition::getCallback() {
        return callback;
    }

    void Transition::setCallback(const std::function<void(SyntaxAnalyzer *)> callback) {
        this->callback = callback;
    }

    // ========= AUTOMATON ========

    bool qBegin(SyntaxAnalyzer * analyzer) {
        Transition * transition = new Transition(AutomatonPattern::pLABEL);
        transition->setState(q1);
        transition->setId(true);
        bool r = analyzer->q(transition);
        if(!r) {
            analyzer->setAux1(((SuccessStatus *)analyzer->getStatus())->getToken());
        }
        return r;
    }

    bool q1(SyntaxAnalyzer * analyzer) {
        Transition * transition = new Transition(AutomatonPattern::pLABEL, TokenTypes::tBLOCKDEF);
        transition->setState(qEnd);
        bool r = analyzer->q(transition);

        if(!r && ((SuccessStatus *)analyzer->getStatus())->getTokenName() == TokenNames::nDirSEGMENT ) {
            return false;
        }
        if( !r && ((SuccessStatus *)analyzer->getStatus())->getTokenName() == TokenNames::nDirMACRO ) {
            analyzer->setState(q1_1);
            analyzer->setMacroScope(true);
            analyzer->setVAux(new std::vector<std::string>());
            return false;
        }
        transition->setTokenType(TokenTypes::tBLOCKEND);
        transition->setLoad(false);
        r = analyzer->q(transition);
        if( !r && ((SuccessStatus *)analyzer->getStatus())->getTokenName() == TokenNames::nDirENDS ) {
            return false;
        }

        std::cout << "Erro q1 2" << std::endl;

        analyzer->setError(true);
        return false;
    }

    bool qEnd(SyntaxAnalyzer * analyzer) {

        if(!((SuccessStatus *)analyzer->getStatus())->isEndOfLine()) {
            analyzer->setError(true);
            std::cout << "FALHA" << std::endl;
        }
        else std::cout << "SUCESSO" << std::endl;
        return true;
    }

    bool q1_1(SyntaxAnalyzer * analyzer) {
        Transition * transition = new Transition(AutomatonPattern::pLABEL);
        transition->setState(q1_1_1);
        transition->setId(true);
        bool r = analyzer->q(transition);
        if(!r) {
            analyzer->getVAux()->emplace_back(((SuccessStatus *)analyzer->getStatus())->getToken());
            if(((SuccessStatus *)analyzer->getStatus())->isEndOfLine()) {
                analyzer->setState(qEnd);
            }
        }
        return r;
    }

    bool q1_1_1(SyntaxAnalyzer * analyzer) {
        Transition * transition = new Transition(AutomatonPattern::pSYMBOL, TokenTypes::tSEPARATOR);
        transition->setState(q1_1);
        return analyzer->q(transition);
    }

}




