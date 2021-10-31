#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <unordered_map>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include "MacroDef.h"
#include "Symbol.h"

class Assembler
{
private:
    std::vector<Semantic *> * lines;

    std::vector<MacroDef *> macroList;
    std::unordered_map<std::string, MacroDef *> macroTable;
    std::unordered_map<std::string, Symbol *> symbolTable;

    std::string output;

    std::vector<unsigned char> assembleCode;

    int lineCounter;
    //int programCounter;

    int assemblerError;

    std::string macroExpandParams(std::vector<Symbol *> params, MacroDef * macroThis);
    int macroExpandParamsDoDaniel(MacroCall * macrocall, int k);

    void replaceAll(std::string& str, const std::string& from, const std::string& to);

    int basicoAssemblerStep1();         //Está presente em Assembler.cpp
    int basicoAssemblerStep2();         //Está presente em Assembler.cpp

    int basicoAssemblerOneStep();       //Não está presente em Assembler.cpp
    
    int completoAssemblerStep1();       //Não está presente em Assembler.cpp
    int completoAssemblerStep2();       //Não está presente em Assembler.cpp

    int loadandgoAssembler();           //Não está presente em Assembler.cpp
    //FAZER: Usar line->getOpcode() e concatenar com o vetor de bytecodes na posição
    //POSIÇÃO AINDA NÃO ESTÁ DETECTÁVEL NO MÉTODO, PRECISA ADICIONAR O ÍNDICE COMO PARÂMETRO AQUI
    std::vector<unsigned char> assembleByteCode(Semantic * line);


public:
    Assembler(std::vector<Semantic *> * lines);

    //Método principal para chamar o pré processador
    std::string preproccess();
    //Método principal para chamar o montador (PRÉPROCESSADOR PRECISA SER CHAMADO ANTES)
    //Recebe como parâmetro o tipo de montador que será executado
    //"0" para montador simples de 1 passo
    //"1" para montador load-and-go
    //"2" para montador simples de 2 passos
    //"3" para montador completo de 2 passos
    //Retorna uma flag de erro.
    int assemble(int assemblerType);

    void init(bool willExecute);
    int preproccessDoDaniel (std::vector<Semantic *> * lines, int k);

    // debug only
    std::string getOutput();

};

#endif // ASSEMBLER_H
