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

        if( !r && analyzer->getLastToken()->getName() == TokenNames::nDirENDM ) {
            if(!analyzer->getMacroStack()->empty()) analyzer->getMacroStack()->pop();
            return false;
        }

        // <macrocontent>
        if(!analyzer->getMacroStack()->empty()) {

            transition->setState(q1);
            transition->setId(true);
            transition->setUndo(true);
            r = analyzer->q(transition);
            transition->~Transition();

            if(!r) return false;

            analyzer->acceptMacroContent();
            analyzer->setError(false);

            return true;
        }
        // </macrocontent>

        transition->setTokenType(TokenTypes::tASSUME);
        transition->setState(q2);
        transition->setLoad(false);
        r = analyzer->q(transition);
        if(!r) return false;

        transition->setTokenType(TokenTypes::tEND);
        transition->setState(q3);
        transition->setLoad(false);
        r = analyzer->q(transition);
        if(!r) return false;

        // variavel
        transition->setTokenType(TokenTypes::tVARDEF);
        transition->setLoad(false);
        transition->setState(q1_dw_valor);
        r = analyzer->q(transition);
        if(!r && analyzer->getLastToken()->getName() == TokenNames::nDW) {
            transition->~Transition();
            return false;
        }
        // org
        transition->setTokenType(TokenTypes::tORG);
        transition->setLoad(false);
        transition->setState(q1_equ_valor);
        r = analyzer->q(transition);
        if(!r && analyzer->getLastToken()->getName() == TokenNames::nORG) {
            transition->~Transition();
            return false;
        }
        // instrucoes
        transition->setTokenType(TokenTypes::tOPERATION);
        transition->setLoad(false);
        transition->setState(q1_equ_valor);
        r = analyzer->q(transition);

        if(!r && analyzer->getLastToken()->getName() == TokenNames::nOpINT) {
            transition->~Transition();
            analyzer->setEndpoint(qEnd);
            return ExpressionAutomaton::qBegin_Expression(analyzer);
        }

        if(!r) {
            switch (analyzer->getLastToken()->getName()) {
                case TokenNames::nOpMOV:
                    analyzer->setState(qMov);
                    break;
                case TokenNames::nOpADD:
                case TokenNames::nOpSUB:
                case TokenNames::nOpCMP:
                    analyzer->setState(qASOA);
                    break;
                case TokenNames::nOpMUL:
                case TokenNames::nOpDIV:
                    analyzer->setState(qMD);
                    break;
                case TokenNames::nOpJMP:
                case TokenNames::nOpJE:
                case TokenNames::nOpJNZ:
                case TokenNames::nOpJZ:
                case TokenNames::nOpJP:
                case TokenNames::nOpCALL:
                    analyzer->setState(qJ);
                    break;
                case TokenNames::nOpRET:
                case TokenNames::nOpPOPF:
                case TokenNames::nOpPUSHF:
                    analyzer->setState(qEnd);
                    break;
                case TokenNames::nOpPOP:
                case TokenNames::nOpPUSH:
                    analyzer->setState(qMov_1_ax);
                    break;
                default:
                    break;
            }
            transition->~Transition();
            return false;
        }

        transition->setTokenType(TokenTypes::tExpLOGICALb);
        transition->setLoad(false);
        transition->setState(qASOA);
        r = analyzer->q(transition);
        if(!r) {
            transition->~Transition();
            return false;
        }

        transition->setTokenType(TokenTypes::tExpLOGICALu);
        transition->setState(qMov_1_ax);
        r = analyzer->q(transition);
        if(!r) {
            transition->~Transition();
            return false;
        }

        transition->setTokenType(TokenTypes::tHALT);
        transition->setState(qEnd);
        r = analyzer->q(transition);
        if(!r) {
            transition->~Transition();
            return false;
        }

        transition->setState(q1);
        transition->setId(true);
        r = analyzer->q(transition);
        if(!r) {
            analyzer->setAux1(analyzer->getLastToken()->getToken());
        }

        transition->~Transition();
        return r;
    }

    bool qJ(SyntaxAnalyzer * analyzer) {
        
        Transition * transition = new Transition(AutomatonPattern::pLABEL);
        transition->setState(qEnd);
        transition->setId(true);
        bool r = analyzer->q(transition);
        transition->~Transition();
        if(!r) {
            analyzer->setAux1(analyzer->getLastToken()->getToken());
        }
        return r;
    }

    bool qMD(SyntaxAnalyzer * analyzer) {
        Transition * transition = new Transition(AutomatonPattern::pLABEL, TokenTypes::tREGISTER);
        transition->setState(qEnd);
        bool r = analyzer->q(transition);
        transition->~Transition();
        if(!r && (
                analyzer->getLastToken()->getName() == TokenNames::nRegAX ||
                analyzer->getLastToken()->getName() == TokenNames::nRegSI
        )) {
            return false;
        }
        analyzer->setError(true);
        return r;
    }

    bool qASOA(SyntaxAnalyzer * analyzer) {
        Transition * transition = new Transition(AutomatonPattern::pLABEL, TokenTypes::tREGISTER);
        transition->setState(qASOA_sep);
        bool r = analyzer->q(transition);
        transition->~Transition();
        if(!r && analyzer->getLastToken()->getName() == TokenNames::nRegAX) return false;
        analyzer->setError(true);
        return false;
    }

    bool qASOA_sep(SyntaxAnalyzer * analyzer) {
        Transition * transition = new Transition(AutomatonPattern::pSYMBOL, TokenTypes::tSEPARATOR);
        transition->setState(qASOA_sep_reg);
        bool r = analyzer->q(transition);
        transition->~Transition();
        return r;
    }

    bool qASOA_sep_reg(SyntaxAnalyzer * analyzer) {
        Transition * transition = new Transition(AutomatonPattern::pLABEL, TokenTypes::tREGISTER);
        transition->setState(qEnd);
        bool r = analyzer->q(transition);
        transition->~Transition();
        if(!r && (
                analyzer->getLastToken()->getName() == TokenNames::nRegAX ||
                analyzer->getLastToken()->getName() == TokenNames::nRegDX
        )) {
            return false;
        }
        analyzer->undoScan();
        analyzer->setEndpoint(qEnd);
        return ExpressionAutomaton::qBegin_Expression(analyzer);
    }

    bool qMov(SyntaxAnalyzer * analyzer) {
        Transition * transition = new Transition(AutomatonPattern::pLABEL, TokenTypes::tREGISTER);
        transition->setState(qMov_1);
        bool r = analyzer->q(transition);

        if(!r && (
            analyzer->getLastToken()->getName() == TokenNames::nRegSP ||
            analyzer->getLastToken()->getName() == TokenNames::nRegSS ||
            analyzer->getLastToken()->getName() == TokenNames::nRegDS ||
            analyzer->getLastToken()->getName() == TokenNames::nRegDX ||
            analyzer->getLastToken()->getName() == TokenNames::nRegSI
        )) {
            analyzer->setAux1(analyzer->getLastToken()->getToken());
            transition->~Transition();
            return false;
        }

        if(!r && analyzer->getLastToken()->getName() == TokenNames::nRegAX) {
            analyzer->setAux1(analyzer->getLastToken()->getToken());
            analyzer->setState(qMov_2);
            transition->~Transition();
            return false;
        }

        transition->setAutomatonPattern(AutomatonPattern::pSYMBOL);
        transition->setTokenType(TokenTypes::tINDEX_OP);
        transition->setState(qMov_3_i_si);
        transition->setUndo(true);
        r = analyzer->q(transition);
        transition->~Transition();
        if(!r) return false;

        analyzer->undoScan();
        analyzer->setEndpoint(qMov_3_exp_i);
        return ExpressionAutomaton::qBegin_Expression(analyzer);
    }

    bool qMov_3_exp_i(SyntaxAnalyzer * analyzer) {
        Transition * transition = new Transition(AutomatonPattern::pSYMBOL, TokenTypes::tINDEX_OP);
        transition->setState(qMov_3_i_si);
        bool r = analyzer->q(transition);
        if(!r) return false;
        transition->~Transition();

        analyzer->undoScan();
        return qMov_1(analyzer);
    }

    bool qMov_3_i_si(SyntaxAnalyzer * analyzer) {
        Transition * transition = new Transition(AutomatonPattern::pLABEL, TokenTypes::tREGISTER);
        transition->setState(qMov_3_i_si_i);
        bool r = analyzer->q(transition);
        transition->~Transition();
        if(!r && analyzer->getLastToken()->getName() == TokenNames::nRegSI) {
            analyzer->setAux2(analyzer->getLastToken()->getToken());
            return false;
        }
        analyzer->setError(true);
        return r;
    }

    bool qMov_3_i_si_i(SyntaxAnalyzer * analyzer) {
        Transition * transition = new Transition(AutomatonPattern::pSYMBOL, TokenTypes::tINDEX_ED);
        transition->setState(qMov_1);
        bool r = analyzer->q(transition);
        transition->~Transition();
        return r;
    }

    bool qMov_1(SyntaxAnalyzer * analyzer) {
        Transition * transition = new Transition(AutomatonPattern::pSYMBOL, TokenTypes::tSEPARATOR);
        transition->setState(qMov_1_ax);
        bool r = analyzer->q(transition);
        transition->~Transition();
        return r;
    }

    bool qMov_1_ax(SyntaxAnalyzer * analyzer) {
        Transition * transition = new Transition(AutomatonPattern::pLABEL, TokenTypes::tREGISTER);
        transition->setState(qEnd);
        bool r = analyzer->q(transition);
        transition->~Transition();
        if(!r && analyzer->getLastToken()->getName() == TokenNames::nRegAX){
            analyzer->setAux2("ax");
            return false;
        }
        analyzer->setError(true);
        return r;
    }

    bool qMov_2(SyntaxAnalyzer * analyzer) {
        Transition * transition = new Transition(AutomatonPattern::pSYMBOL, TokenTypes::tSEPARATOR);
        transition->setState(qMov_2_f);
        bool r = analyzer->q(transition);
        transition->~Transition();
        return r;
    }

    bool qMov_2_f(SyntaxAnalyzer * analyzer) {
        Transition * transition = new Transition(AutomatonPattern::pLABEL, TokenTypes::tREGISTER);
        transition->setState(qEnd);
        bool r = analyzer->q(transition);

        if(!r && (
            analyzer->getLastToken()->getName() == TokenNames::nRegSP ||
            analyzer->getLastToken()->getName() == TokenNames::nRegSS ||
            analyzer->getLastToken()->getName() == TokenNames::nRegDS ||
            analyzer->getLastToken()->getName() == TokenNames::nRegDX ||
            analyzer->getLastToken()->getName() == TokenNames::nRegSI
        )) {
            analyzer->setAux2(analyzer->getLastToken()->getToken());
            transition->~Transition();
            return false;
        }

        transition->setUndo(true);
        transition->setAutomatonPattern(AutomatonPattern::pSYMBOL);
        transition->setTokenType(TokenTypes::tINDEX_OP);
        transition->setState(qMov_2_i_si);
        r = analyzer->q(transition);
        if(!r) {
            transition->~Transition();
            return false;
        }

        transition->~Transition();

        analyzer->setEndpoint(qMov_2_exp_i);
        return ExpressionAutomaton::qBegin_Expression(analyzer);
    }

    bool qMov_2_exp_i(SyntaxAnalyzer * analyzer) {

        if(analyzer->getLastToken()->isEndOfLine()) {
            analyzer->setState(qEnd);
            return false;
        }

        Transition * transition = new Transition(AutomatonPattern::pSYMBOL, TokenTypes::tINDEX_OP);
        transition->setState(qMov_2_i_si);
        bool r = analyzer->q(transition);
        transition->~Transition();
        return r;
    }

    bool qMov_2_i_si(SyntaxAnalyzer * analyzer) {
        Transition * transition = new Transition(AutomatonPattern::pLABEL, TokenTypes::tREGISTER);
        transition->setState(qMov_2_i_si_i);
        bool r = analyzer->q(transition);
        transition->~Transition();
        if(!r && analyzer->getLastToken()->getName() == TokenNames::nRegSI) {
            analyzer->setAux2(analyzer->getLastToken()->getToken());
            return false;
        }
        analyzer->setError(true);
        return r;
    }

    bool qMov_2_i_si_i(SyntaxAnalyzer * analyzer) {
        Transition * transition = new Transition(AutomatonPattern::pSYMBOL, TokenTypes::tINDEX_ED);
        transition->setState(qEnd);
        bool r = analyzer->q(transition);
        transition->~Transition();
        return r;
    }

    // -> <ASSUME> ->
    bool q2(SyntaxAnalyzer * analyzer) {
        Transition * transition = new Transition(AutomatonPattern::pLABEL, TokenTypes::tREGISTER);
        transition->setState(q2_col);
        bool r = analyzer->q(transition);
        if(!r && (
                    analyzer->getLastToken()->getName() == TokenNames::nRegCS ||
                    analyzer->getLastToken()->getName() == TokenNames::nRegDS ||
                    analyzer->getLastToken()->getName() == TokenNames::nRegSS
        )) {
            analyzer->setAux1(analyzer->getLastToken()->getToken());
            return false;
        }
        return r;
    }

    // -> <ASSUME> -> <registrador> ->
    bool q2_col(SyntaxAnalyzer * analyzer) {
        Transition * transition = new Transition(AutomatonPattern::pSYMBOL, TokenTypes::tCOLON);
        transition->setState(q2_col_id);
        return analyzer->q(transition);
    }

    // -> <ASSUME> -> <registrador> -> <:> ->
    bool q2_col_id(SyntaxAnalyzer * analyzer) {
        Transition * transition = new Transition(AutomatonPattern::pLABEL);
        transition->setState(qEnd);
        transition->setId(true);
        bool r = analyzer->q(transition);
        transition->~Transition();
        if(!r) {
            analyzer->setAux2(analyzer->getLastToken()->getToken());
        }
        return r;
    }

    // -> <END ->
    bool q3(SyntaxAnalyzer * analyzer) {
        Transition * transition = new Transition(AutomatonPattern::pLABEL);
        transition->setState(qEnd);
        transition->setId(true);
        bool r = analyzer->q(transition);
        if(!r) {
            analyzer->setAux1(analyzer->getLastToken()->getToken());
        }
        return r;
    }

    // -> <identificador> ->
    bool q1(SyntaxAnalyzer * analyzer) {
        Transition * transition = new Transition(AutomatonPattern::pLABEL, TokenTypes::tBLOCKDEF);
        transition->setState(qEnd);
        bool r = analyzer->q(transition);

        // definicao
        if( !r && analyzer->getLastToken()->getName() == TokenNames::nDirMACRO ) {
            analyzer->setState(q1_label);
            analyzer->getMacroStack()->push("m");
            analyzer->setVAux(new std::vector<std::string>());
            transition->~Transition();
            return false;
        }

        // <macrocontent>
        if(!analyzer->getMacroStack()->empty()) {

            analyzer->acceptMacroContent();

            analyzer->setError(false);
            transition->~Transition();
            return true;
        }
        // </macrocontent>

        if(!r && (
                analyzer->getLastToken()->getName() == TokenNames::nDirSEGMENT ||
                analyzer->getLastToken()->getName() == TokenNames::nDirPROC
        )) {
            if(analyzer->getLastToken()->getName() == TokenNames::nDirSEGMENT && !analyzer->getLastToken()->isEndOfLine()) {
                analyzer->setState(q1_stack);
            }
            transition->~Transition();
            return false;
        }
        // fim de definicao
        transition->setTokenType(TokenTypes::tBLOCKEND);
        transition->setLoad(false);
        r = analyzer->q(transition);
        if(!r && (
                analyzer->getLastToken()->getName() == TokenNames::nDirENDS ||
                analyzer->getLastToken()->getName() == TokenNames::nDirENDP
        )) {
            transition->~Transition();
            return false;
        }
        // variavel
        transition->setTokenType(TokenTypes::tVARDEF);
        transition->setLoad(false);
        transition->setState(q1_dw_valor);
        r = analyzer->q(transition);
        if(!r && analyzer->getLastToken()->getName() == TokenNames::nDW) {
            transition->~Transition();
            return false;
        }
        // constante
        transition->setTokenType(TokenTypes::tCONSTDEF);
        transition->setLoad(false);
        transition->setState(q1_equ_valor);
        r = analyzer->q(transition);
        if(!r && analyzer->getLastToken()->getName() == TokenNames::nEQU) {
            transition->~Transition();
            return false;
        }
        transition->~Transition();

        transition = new Transition(AutomatonPattern::pSYMBOL, TokenTypes::tCOLON);
        transition->setState(qEnd);
        transition->setUndo(true);
        transition->~Transition();
        r = analyzer->q(transition);
        if(!r) {
            analyzer->setState(qEnd);
            if(!analyzer->getLastToken()->isEndOfLine()) {
                analyzer->setState(qBegin);
            }

            return false;
        }

        //analyzer->setEndpoint(q1_expression_separator);
        //r = ExpressionAutomaton::qBegin_Expression(analyzer);

        //if(!r && ((SuccessStatus *)analyzer->getStatus())->isEndOfLine()) {
        //    analyzer->setState(qEnd);
        //}
        analyzer->undoScan();
        return q1_expression(analyzer);
    }

    bool q1_stack(SyntaxAnalyzer * analyzer) {
        Transition * transition = new Transition(AutomatonPattern::pLABEL, TokenTypes::tSTACK);
        transition->setState(qEnd);
        bool r = analyzer->q(transition);
        transition->~Transition();
        return r;
    }

    bool q1_equ_valor(SyntaxAnalyzer * analyzer) {
        analyzer->setEndpoint(qEnd);
        return ExpressionAutomaton::qBegin_Expression(analyzer);
    }

    bool q1_dw_valor(SyntaxAnalyzer * analyzer) {
        Transition * transition = new Transition(AutomatonPattern::pSYMBOL, TokenTypes::tUNDEFINED);
        transition->setState(qEnd);
        bool r = analyzer->q(transition);
        transition->~Transition();
        if(!r) return false; // '?'
        // com expressao?
        analyzer->undoScan();
        analyzer->setEndpoint(q1_dw_valor_dup);
        r = ExpressionAutomaton::qBegin_Expression(analyzer);
        bool eol = analyzer->getLastToken()->isEndOfLine();
        if(!r && eol) {
            analyzer->setState(qEnd);
        }
        return r;
    }

    bool q1_dw_valor_dup(SyntaxAnalyzer * analyzer) {
        Transition * transition = new Transition(AutomatonPattern::pLABEL, TokenTypes::tVECFILL);
        transition->setState(q1_dw_valor_dup_p);
        bool r = analyzer->q(transition);
        transition->~Transition();
        return r;
    }

    bool q1_dw_valor_dup_p(SyntaxAnalyzer * analyzer) {
        Transition * transition = new Transition(AutomatonPattern::pSYMBOL, TokenTypes::tExpPRECEDENCE_OP);
        transition->setState(q1_dw_valor_dup_p_valor);
        bool r = analyzer->q(transition);
        transition->~Transition();
        return r;
    }

    bool q1_dw_valor_dup_p_valor(SyntaxAnalyzer * analyzer) {
        analyzer->setEndpoint(q1_dw_valor_dup_p_valor_p);
        return ExpressionAutomaton::qBegin_Expression(analyzer);
    }

    bool q1_dw_valor_dup_p_valor_p(SyntaxAnalyzer * analyzer) {
        Transition * transition = new Transition(AutomatonPattern::pSYMBOL, TokenTypes::tExpPRECEDENCE_ED);
        transition->setState(qEnd);
        bool r = analyzer->q(transition);
        transition->~Transition();
        return r;
    }


    bool q1_label(SyntaxAnalyzer * analyzer) {
        Transition * transition = new Transition(AutomatonPattern::pLABEL);
        transition->setState(q1_label_separator);
        transition->setId(true);
        bool r = analyzer->q(transition);
        if(!r) {
            analyzer->getVAux()->emplace_back(analyzer->getLastToken()->getToken());
            if(analyzer->getLastToken()->isEndOfLine()) {
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
        analyzer->setEndpoint(q1_expressionAux);
        bool r = ExpressionAutomaton::qBegin_Expression(analyzer);
        bool eol = analyzer->getLastToken()->isEndOfLine();
        if(!r && eol) {
            analyzer->setState(qEnd);
        }
        return r;
    }

    bool q1_expressionAux(SyntaxAnalyzer * analyzer) {
        bool eol = analyzer->getLastToken()->isEndOfLine();
        if(eol) {
            analyzer->setState(qEnd);
            return false;
        }

        analyzer->setState(q1_expression_separator);
        return false;
    }

    bool q1_expression_separator(SyntaxAnalyzer * analyzer) {
        Transition * transition = new Transition(AutomatonPattern::pSYMBOL, TokenTypes::tSEPARATOR);
        transition->setState(q1_expression);
        return analyzer->q(transition);
    }

    bool qEnd(SyntaxAnalyzer * analyzer) {

        if(!analyzer->getLastToken()->isEndOfLine()) {
            analyzer->setError(true);

            TEST(std::cout << "FALHA" << std::endl)
        }
        else TEST(std::cout << "SUCESSO" << std::endl)

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
            transition->setState(q1_Expression);
            transition->setPop("p");
            bool r = analyzer->q(transition);
            transition->~Transition();
            if(!r) return false; // ')'
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

            bool r = analyzer->q(transition);
            if(!r) {transition->~Transition(); return false;} // 1a5h ou 1a5H

            transition->setUndo(true);
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
            Transition * transition = new Transition(AutomatonPattern::pSYMBOL, TokenTypes::tExpARITHMETICu);
            transition->setState(qBegin_Expression);
            bool r = analyzer->q(transition);
            if(!r) {transition->~Transition(); return false;} // '+' ou '-'
            transition->~Transition();

            analyzer->undoScan();
            transition = new Transition(AutomatonPattern::pSYMBOL, TokenTypes::tExpARITHMETICb);
            transition->setState(qBegin_Expression);

            r = analyzer->q(transition);
            if(!r) {transition->~Transition(); return false;} // '*' ou '/'

            transition->setUndo(true);
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
