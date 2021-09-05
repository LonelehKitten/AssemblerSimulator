#ifndef LEXICON_SCANNER_HPP
#define LEXICON_SCANNER_HPP

#include <iostream>
#include <string>
#include <functional>
#include <unordered_map>
#include <stack>

#include "LexiconScannerStatus.hpp"
#include "Automatons.hpp"

class LexiconScanner {

    public:
        /*
            Type that tokens are classified in
        */
        typedef enum TokenTypes {
            NULL_TYPE = -1,
            OPERATOR,
            OPERAND,
            LITERAL
        } TokenTypes;

    private:

        /**
         * Ponteiro para os estados dos autômatos
         * @returns - true se o token for aceito, false caso contrário
         */
        std::function<bool()> state;

        std::unordered_map<std::string, LexiconScanner::TokenTypes> tokens;
        std::unordered_map<int, std::function<bool()>> initialStates;

        std::stack<std::string> * stack;

        LexiconScannerStatus * tokenData;

        /**
         * Caractere atualmente lido de line
         */
        char currentChar;

        /**
         * Linha a ser lida
         */
        std::string line;

        std::string token;

        bool error;

        /**
         * Flag que indica quando o cabeçote chegou no final da linha
         */
        bool endOfLine;

        /**
         * Posição do cabeçote na linha
         */
        int lineIndex;

        /**
         * Posição do primeiro caractere do último token lido na linha
         */
        int lastTokenBeginPosition;

        /**
         * Posição do último caractere do último token lido na linha
         */
        int lastTokenEndPosition;


        void start(int tokenType);

        void snap();
        
        void log();

        void nextChar();

        void checkEndOfLine(bool deterministic);

        void accept(bool deterministic=true);

        void setSuccessMessage(LexiconScanner::TokenTypes tokenType);

    public:

        typedef struct TokenSetUnit {
            private:
                std::string token;
                LexiconScanner::TokenTypes tokenType;
            public:
                TokenSetUnit(std::string token, LexiconScanner::TokenTypes tokenType);
                std::string getToken();
                LexiconScanner::TokenTypes getTokenType();
        } TokenSetUnit;

        struct TokenSet {
            public:
                static TokenSetUnit * ADD;
        };

        /**
         * Seta a próxima linha a ser lida e reseta as configurações do analisador léxico
         * @param line linha a ser lida
         */
        void setLine(std::string line);

        /**
         * desfaz a ultima leitura
         * Alert: apenas 1 vez
         */
        void undo();

        /**
         * Lê o próximo token de line usando o estado inicial referente ao tipo de token informado
         * 
         * @param tokenType O tipo do próximo token que deve ser reconhecido
         * @returns Retorna o token reconhecido, seu tipo, o indicador de final de linha e qual analisador lexico foi utilisado
         */
        LexiconScannerStatus * nextToken(TokenTypes tokenType);

        // ======================

        bool q(Automatons::Transition ** transition, int length, Automatons::Transition * defaultAction=nullptr);

        bool qEnd(Automatons::TransitionEnd * transitionEnd);


        // =============== comparadores ================

        bool isStackValue(std::string value);

        // verifica se o token é reconhecido pelo dicionário, caso contrário pode ser um identificador ou indicar erro
        bool isUnknownToken();

        // verifica se caractere corrente é igual a um caractere específico
        bool is(char character);

        // conjunto α U Α (alfa minúsculo união alfa maiúsculo)
        bool isAlpha();

        bool isAlphaUpper();

        bool isAlphaLower();
        
        // conjunto Γ (gama)
        bool isNumeric();

        // conjunto Γ (gama) ou α U Α (alfa minúsculo união alfa maiúsculo)
        bool isAlphaNumeric();
        
        // valor ε (epsilon)
        bool isNull();

        // valor Φ (fi)
        bool isLineBreak();

        // conjunto Σ (sigma)
        bool isSpecial();

        // =============================================

};

#endif /* LEXICON_SCANNER_HPP */