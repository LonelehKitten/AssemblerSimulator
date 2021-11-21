#ifndef SYNTAXAUTOMATONS_H
#define SYNTAXAUTOMATONS_H

#include <functional>
#include <string>
#include <iostream>
#include <vector>

#include "Dictionaries.h"

class SyntaxAnalyzer;

namespace SyntaxAutomatons {

    class Transition {

        private:
            AutomatonPattern automatonPattern;
            TokenTypes tokenType;
            std::function<bool(SyntaxAnalyzer *)> state;
            std::string token, pop, push;
            bool customFlag, load, id, undo;
            std::function<void(SyntaxAnalyzer *)> callback;

        public:
            Transition(AutomatonPattern automatonPattern, TokenTypes tokenTypes=TokenTypes::tNULL_TYPE);


            AutomatonPattern getAutomatonPattern();
            TokenTypes getTokenType();
            const std::function<bool(SyntaxAnalyzer *)> getState();
            std::string getToken();
            std::string getPop();
            std::string getPush();
            bool getCustomFlag();
            bool getLoad();
            bool isId();
            bool mustUndo();
            const std::function<void(SyntaxAnalyzer *)> getCallback();

            void setAutomatonPattern(AutomatonPattern automatonPattern);
            void setTokenType(TokenTypes tokenType);
            void setState(const std::function<bool(SyntaxAnalyzer *)> state);
            void setToken(std::string token);
            void setPop(std::string pop);
            void setPush(std::string push);
            void setCustomFlag(bool customFlag);
            void setLoad(bool load);
            void setId(bool id);
            void setUndo(bool undo);
            void setCallback(const std::function<void(SyntaxAnalyzer *)> callback);
    };

    bool qBegin(SyntaxAnalyzer * analyzer);
    bool qMov(SyntaxAnalyzer * analyzer);
    bool qMov_1(SyntaxAnalyzer * analyzer);
    bool qMov_1_ax(SyntaxAnalyzer * analyzer);
    bool qMov_2(SyntaxAnalyzer * analyzer);
    bool qMov_2_f(SyntaxAnalyzer * analyzer);
    bool qMov_2_exp_i(SyntaxAnalyzer * analyzer);
    bool qMov_2_i_si(SyntaxAnalyzer * analyzer);
    bool qMov_2_i_si_i(SyntaxAnalyzer * analyzer);
    bool qMov_3_exp_i(SyntaxAnalyzer * analyzer);
    bool qMov_3_i_si(SyntaxAnalyzer * analyzer);
    bool qMov_3_i_si_i(SyntaxAnalyzer * analyzer);
    bool qASOA(SyntaxAnalyzer * analyzer);
    bool qASOA_sep(SyntaxAnalyzer * analyzer);
    bool qASOA_sep_reg(SyntaxAnalyzer * analyzer);
    bool qMD(SyntaxAnalyzer * analyzer);
    bool qJ(SyntaxAnalyzer * analyzer);
    bool q1(SyntaxAnalyzer * analyzer);
    bool q1_stack(SyntaxAnalyzer * analyzer);
    bool q1_extrn_label(SyntaxAnalyzer * analyzer);
    bool q1_extrn_label_c(SyntaxAnalyzer * analyzer);
    bool q1_extrn_label_c_t(SyntaxAnalyzer * analyzer);
    bool q1_extrn_label_c_t_s(SyntaxAnalyzer * analyzer);
    bool q1_label(SyntaxAnalyzer * analyzer);
    bool q1_dw_valor(SyntaxAnalyzer * analyzer);
    bool q1_dw_valor_maybedup(SyntaxAnalyzer * analyzer);
    bool q1_dw_valor_dup(SyntaxAnalyzer * analyzer);
    bool q1_dw_valor_dup_p(SyntaxAnalyzer * analyzer);
    bool q1_dw_valor_dup_p_valor(SyntaxAnalyzer * analyzer);
    bool q1_dw_valor_dup_p_valor_p(SyntaxAnalyzer * analyzer);
    bool q1_equ_valor(SyntaxAnalyzer * analyzer);
    bool q1_label_separator(SyntaxAnalyzer * analyzer);
    bool q1_expression(SyntaxAnalyzer * analyzer);
    bool q1_expressionAux(SyntaxAnalyzer * analyzer);
    bool q1_expression_separator(SyntaxAnalyzer * analyzer);
    bool q2(SyntaxAnalyzer * analyzer);
    bool q2_col(SyntaxAnalyzer * analyzer);
    bool q2_col_id(SyntaxAnalyzer * analyzer);
    bool q3(SyntaxAnalyzer * analyzer);

    bool qEnd(SyntaxAnalyzer * analyzer);

    namespace ExpressionAutomaton {

        bool qBegin_Expression(SyntaxAnalyzer * analyzer);
        bool q1_Expression(SyntaxAnalyzer * analyzer);

        bool qOperand_Expression(SyntaxAnalyzer * analyzer);
        bool qUOperator_Expression(SyntaxAnalyzer * analyzer);
        bool qBOperator_Expression(SyntaxAnalyzer * analyzer);
    }
};

#endif // SYNTAXAUTOMATONS_H
