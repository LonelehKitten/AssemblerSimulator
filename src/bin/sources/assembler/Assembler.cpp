#include "Assembler.h"

Assembler::Assembler(std::vector<Semantic *> * lines) :
    lines(lines)
{}

std::string Assembler::preproccess() {
    
    /**Ja definido em Assembler.h: 
    *       std::vector<macroDef *> macroTable;
    *       std::vector<label *> labelTable;
    */

    std::vector<bool> macroFound;   //Pilha de controle das declaracoes de macro


    for (int i = 0; i < (int) lines->size(); i++)   //Leitura principal de arquivos
        {
            Semantic *Item = (*lines)[i];
            std::cout << Item->getLine() << std::endl;
            if (Item->getType() == iMACRO)
            {
                if (macroFound.size() == 0)
                {

                    Macro * itemMacro = (Macro *)((*lines)[i]);
                    macroTable.push_back(new macroDef(itemMacro->getName(), *(itemMacro->getParams())));
                    macroFound.push_back(true);
                }
                else
                {
                    macroFound.push_back(true);
                }
            }

            else if (Item->getType() == iENDM)
            {
                //   MNormal();
                if (macroFound.size() == 0)
                {
                    // Erro
                    std::cout << "Error ";
                }else{
                    macroFound.pop_back();
                }

            }
            else if (Item->getType() == Instruction::iMACROCALL)
            {
                // IterarMacroCall();
            }
            /*else if (macroFound.size() == 0){
            //    std::cout << "Error ";
                // Error
            }*/
            else
            {
                macroTable.back()->setText(Item->getLine());
            }
        }

        //Teste da tabelacao de macro
        std::cout << "Total: " << macroTable.size() << std::endl << std::endl;
        for (int i = 0; i < (int) macroTable.size(); i++)
        {
            macroDef * item = macroTable[i];
            std::cout << item->getText() << std::endl;
        }

        return "AQUI DEVE RETORNAR O TEXTO PROCESSADO QUE VAI SER DEVOLVIDO PARA A INTERFACE";

}
