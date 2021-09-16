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
        undo(false),
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

    bool Transition::mustUndo() {
        return undo;
    }

    void Transition::setUndo(bool undo) {
        this->undo = undo;
    }

    const std::function<void(SyntaxAnalyzer *)> Transition::getCallback() {
        return callback;
    }

    void Transition::setCallback(const std::function<void(SyntaxAnalyzer *)> callback) {
        this->callback = callback;
    }

    // ========= AUTOMATON ========

    bool qBegin(SyntaxAnalyzer * analyzer) {
        Transition * transition = new Transition(AutomatonPattern::pLABEL, TokenTypes::tBLOCKEND);
        transition->setState(qEnd);
        bool r = analyzer->q(transition);

        if( !r && ((SuccessStatus *)analyzer->getStatus())->getTokenName() == TokenNames::nDirENDM ) {
            analyzer->setMacroScope(false);
            return false;
        }

        transition->setState(q1);
        transition->setLoad(false);
        transition->setId(true);
        r = analyzer->q(transition);
        if(!r) {
            analyzer->setAux1(((SuccessStatus *)analyzer->getStatus())->getToken());
        }

        return r;
    }

    // -> <identificador> ->
    bool q1(SyntaxAnalyzer * analyzer) {
        Transition * transition = new Transition(AutomatonPattern::pLABEL, TokenTypes::tBLOCKDEF);
        transition->setState(qEnd);
        bool r = analyzer->q(transition);

        // definicao
        if(!r && (
                ((SuccessStatus *)analyzer->getStatus())->getTokenName() == TokenNames::nDirSEGMENT ||
                ((SuccessStatus *)analyzer->getStatus())->getTokenName() == TokenNames::nDirPROC
        )) {
            transition->~Transition();
            return false;
        }
        if( !r && ((SuccessStatus *)analyzer->getStatus())->getTokenName() == TokenNames::nDirMACRO ) {
            analyzer->setState(q1_label);
            analyzer->setMacroScope(true);
            analyzer->setVAux(new std::vector<std::string>());
            transition->~Transition();
            return false;
        }
        // fim de definicao
        transition->setTokenType(TokenTypes::tBLOCKEND);
        transition->setLoad(false);
        r = analyzer->q(transition);
        if(!r && (
                ((SuccessStatus *)analyzer->getStatus())->getTokenName() == TokenNames::nDirENDS ||
                ((SuccessStatus *)analyzer->getStatus())->getTokenName() == TokenNames::nDirENDP
        )) {
            transition->~Transition();
            return false;
        }
        transition->~Transition();

        transition = new Transition(AutomatonPattern::pSYMBOL, TokenTypes::tCOLON);
        transition->setState(qEnd);
        transition->setUndo(true);
        transition->~Transition();
        r = analyzer->q(transition);
        if(!r) return false;

        analyzer->setEndpoint(q1_expression_separator);
        r = ExpressionAutomaton::qBegin_Expression(analyzer);

        if(!r && ((SuccessStatus *)analyzer->getStatus())->isEndOfLine()) {
            analyzer->setState(qEnd);
        }

        return r;
    }


    bool q1_label(SyntaxAnalyzer * analyzer) {
        Transition * transition = new Transition(AutomatonPattern::pLABEL);
        transition->setState(q1_label_separator);
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

    bool q1_label_separator(SyntaxAnalyzer * analyzer) {
        Transition * transition = new Transition(AutomatonPattern::pSYMBOL, TokenTypes::tSEPARATOR);
        transition->setState(q1_label);
        return analyzer->q(transition);
    }

    bool q1_expression(SyntaxAnalyzer * analyzer) {
        analyzer->setEndpoint(q1_expression_separator);
        bool r = ExpressionAutomaton::qBegin_Expression(analyzer);
        if(!r && ((SuccessStatus *)analyzer->getStatus())->isEndOfLine()) {
            analyzer->setState(qEnd);
        }
        return r;
    }

    bool q1_expression_separator(SyntaxAnalyzer * analyzer) {
        Transition * transition = new Transition(AutomatonPattern::pSYMBOL, TokenTypes::tSEPARATOR);
        transition->setState(q1_expression);
        return analyzer->q(transition);
    }

    bool qEnd(SyntaxAnalyzer * analyzer) {

        if(!((SuccessStatus *)analyzer->getStatus())->isEndOfLine()) {
            analyzer->setError(true);
            std::cout << "FALHA" << std::endl;
        }
        else std::cout << "SUCESSO" << std::endl;
        return true;
    }

    namespace ExpressionAutomaton {

        // ->
        bool qBegin_Expression(SyntaxAnalyzer * analyzer) {
            Transition * transition = new Transition(AutomatonPattern::pSYMBOL, TokenTypes::tExpPRECEDENCE_OP);
            transition->setState(qBegin_Expression);
            transition->setPush("p");
            bool r = analyzer->q(transition);
            if(!r) return false; // '('
            transition->~Transition();
            analyzer->undoScan();
            if(!qUOperator_Expression(analyzer)) return false;
            analyzer->undoScan();
            return qOperand_Expression(analyzer);
        }

        // operando ->
        bool q1_Expression(SyntaxAnalyzer * analyzer) {
            Transition * transition = new Transition(AutomatonPattern::pSYMBOL, TokenTypes::tExpPRECEDENCE_ED);
            transition->setState(qBegin_Expression);
            transition->setPop("p");
            bool r = analyzer->q(transition);
            transition->~Transition();
            if(!r) return false; // '('
            analyzer->undoScan();
            if(!qBOperator_Expression(analyzer)) return false;
            analyzer->undoScan();
            analyzer->setState(analyzer->getEndpoint());
            return !analyzer->getStack()->empty();
        }

        // utils

        bool qOperand_Expression(SyntaxAnalyzer * analyzer) {
            Transition * transition = new Transition(AutomatonPattern::pHEXADECIMAL);
            transition->setState(q1_Expression);
            transition->setId(true);
            transition->setUndo(true);
            bool r = analyzer->q(transition);
            if(!r) {transition->~Transition(); return false;} // 1a5h ou 1a5H

            transition->setAutomatonPattern(AutomatonPattern::pBINARY);
            r = analyzer->q(transition);
            if(!r) {transition->~Transition(); return false;} // 0101b ou 0101B

            transition->setAutomatonPattern(AutomatonPattern::pDECIMAL);
            r = analyzer->q(transition);
            if(!r) {transition->~Transition(); return false;} // 123D , 123d ou 123

            transition->setAutomatonPattern(AutomatonPattern::pCHARATERE);
            r = analyzer->q(transition);
            if(!r) {transition->~Transition(); return false;} // 'a' ou '\a'

            transition->setAutomatonPattern(AutomatonPattern::pLABEL);
            transition->setTokenType(TokenTypes::tOFFSET);
            transition->setId(false);
            r = analyzer->q(transition); // offset ...
            if(!r) transition->setUndo(false);
            transition->setId(true);
            r = analyzer->q(transition); // [offset] <identificador>
            transition->~Transition();
            return r;
        }

        bool qUOperator_Expression(SyntaxAnalyzer * analyzer) {
            Transition * transition = new Transition(AutomatonPattern::pSYMBOL, TokenTypes::tExpARITHMETICu);
            transition->setState(qBegin_Expression);
            bool r = analyzer->q(transition);
            if(!r) {transition->~Transition(); return false;} // '+' ou '-'
            analyzer->undoScan();
            transition->setAutomatonPattern(AutomatonPattern::pLABEL);
            transition->setTokenType(TokenTypes::tExpLOGICALu);
            r = analyzer->q(transition); // 'not'
            transition->~Transition();
            return r;
        }

        bool qBOperator_Expression(SyntaxAnalyzer * analyzer) {
            if(!qUOperator_Expression(analyzer)) return false; // '+' , '-' ou 'not'
            analyzer->undoScan();
            Transition * transition = new Transition(AutomatonPattern::pSYMBOL, TokenTypes::tExpARITHMETICb);
            transition->setState(qBegin_Expression);
            transition->setUndo(true);
            bool r = analyzer->q(transition);
            if(!r) {transition->~Transition(); return false;} // '*' ou '/'

            transition->setAutomatonPattern(AutomatonPattern::pLABEL);
            r = analyzer->q(transition);
            if(!r) {transition->~Transition(); return false;} // 'mod'

            transition->setTokenType(TokenTypes::tExpLOGICALb);
            r = analyzer->q(transition);
            if(!r) {transition->~Transition(); return false;} // 'or' , 'and' ou 'xor'

            transition->setTokenType(TokenTypes::tExpRELATIONALb);
            r = analyzer->q(transition); // 'eq' , 'ne' , 'lt' , 'le' , 'gt' ou 'ge'
            transition->~Transition();
            return r;
        }

    }
}




