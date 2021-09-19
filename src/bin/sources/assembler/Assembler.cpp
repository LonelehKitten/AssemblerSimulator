#include "Assembler.h"

Assembler::Assembler(std::vector<Semantic *> * lines) :
    lines(lines)
{}

std::string Assembler::macroExpandParams(std::vector<label *> params, macroDef * macroThis)
{
    
    /**Ja definido em Assembler.h: 
    *       std::vector<macroDef *> macroTable;
    *       std::vector<label *> labelTable;
    */

   std::vector<Semantic *> macroLines = macroThis->getText();

   std::string lineThis;
   std::string outputText = "";

   std::vector<bool> macroFound;

   int antes, depois;

   for (int i = 0; i < macroLines.size(); i++)
   {
        if(macroLines[i]->getType() == Instruction::iMACRO)     //Precisa tabelar a macro
        {                                                       //MACRO ANINHADA 

                macroFound.push_back(true);
        }
        else if (macroLines[i]->getType() == Instruction::iENDM)
        {
            if (macroFound.size() == 0)
            {
                // Erro
                std::cout << "Erro: macro mal declarada.";
                break;
            }else
            {
                macroFound.pop_back();
            }
        }
        else     //Nao eh instrucao de definicao de macro
        {
            if (macroFound.size() == 0)     //Nao esta dentro de macro aninhada
            {
                lineThis = macroLines[i]->getLine();
                
                for (int iter = 0; iter < params.size(); iter++)
                {
                    antes = ( lineThis.find(params[iter]->name) ) - 1;
                    depois = ( lineThis.find(params[iter]->name) ) + 1;

                    if(antes >= -1 && depois <= lineThis.size()+1)       // para validar que é uma posição válida
                    {                                       
                        if ((lineThis[antes] == ' ' || lineThis[antes] == ',') && (lineThis[depois] == ' ' || lineThis[depois] == ',' || lineThis[depois] == '\n')){
                            lineThis.replace(lineThis.find(params[iter]->name), params[iter]->name.size(), params[iter]->valor);
                        }
                    }
                }

                outputText += lineThis;
                
            }   
        }
    }

    return outputText;

}

std::string Assembler::preproccess() {
    
    /**Ja definido em Assembler.h: 
     *       std::vector<macroDef *> macroTable;
     *       std::vector<label *> labelTable;
     */

    std::string outputResult = "";

    RecognitionManager * rm = new RecognitionManager();

    std::vector<bool> macroFound;   //Pilha de controle das declaracoes de macro


    for (int i = 0; i < (int) lines->size(); i++)   //Leitura principal do arquivo
    {
        Semantic *Item = (*lines)[i];
        std::cout << Item << std::endl;

        if (Item->getType() == iMACRO)          //Definição de macro
        {
            if (macroFound.size() == 0)
            {

                Macro * itemMacro = (Macro *)((*lines)[i]);
                macroTable.push_back(new macroDef(itemMacro->getName(), *(itemMacro->getParams())));
                macroFound.push_back(true);
            }
            else
            {
                macroTable.back()->setText(Item);
                macroFound.push_back(true);
            }
        }

        else if (Item->getType() == iENDM)
        {
            
            if (macroFound.size() == 0)
            {
                // Erro
                std::cout << "Erro: macro mal declarada.";
                break;
            }else{

                macroTable.back()->setText(Item);
                macroFound.pop_back();

            }

        }
        else if (Item->getType() == Instruction::iMACROCALL)
        {
            if (macroFound.size() == 0) //Nao esta no processo de tabelamento
            {                           //Inicia passo 1; salvar os parametros da chamada
                bool macroExists = false;

                MacroCall * itemMacroCall = (MacroCall *)((*lines)[i]);

                macroDef *macroThis;    //Macro que estava na tabela

                for (std::vector<macroDef*>::iterator it = macroTable.begin(); it != macroTable.end(); it++)   //Procura macro na tabela
                {
                    if( itemMacroCall->getName() == (*it)->getName() )
                    {
                        macroThis = *it;
                        macroExists = true;
                        std::cout << "passou por aqui no teste, busca na tabela" << std::endl;
                    }
                }
                if (!macroExists)
                {
                    std::cout << "Erro: macro nao encontrada na tabela." << std::endl;
                    break;
                }
                
                std::vector<label *> params;

                for (int iter = 0; iter < macroThis->getArgs().size(); iter++)   //Criando a lista de parâmetros
                {
                    params.push_back(new label);
                    params.back()->name = macroThis->getArgs()[iter];
                    for (int iterToken = 0; iterToken < ((MacroCall*) Item)->getParams()->at(iter)->size(); iterToken++)
                    {
                        std::string tokenResult;

                        params.back()->valor = tokenResult;
                    }
                }
                std::cout << "passou por aqui no teste" << std::endl;

                //Começar expansão

                std::string textOnExpansion = "";
                //std::string lineResult;

                //for (int iter = 0; iter < macroThis->getText().size(); iter++)
                //{
                    //Percorrer cada linha de string da macro e substituir as palavras com os parametros
                    //Guardar a macro com as palavras substituidas em lineResult

                //lineResult = macroExpandParams(params, macroThis);
                
                textOnExpansion = macroExpandParams(params, macroThis);
                
                //textOnExpansion += lineResult + '\n';
                //}
                /*
                std::vector<Semantic *> * s_array = rm->analyze(textOnExpansion, false);
                std::vector<Semantic *> * prior = new std::vector(lines->begin(), lines->begin()+i-1);
                std::vector<Semantic *> * after = new std::vector(lines->begin()+i+1, lines->end());
                std::vector<Semantic *> * tempDelete = lines;
                delete tempDelete;
                std::copy(prior->begin(),prior->end(),lines->begin());
                std::copy(s_array->begin(),s_array->end(),lines->end());
                std::copy(after->begin(),after->end(),lines->end());
                */  
                //for(int x=0; x < (*s_array).size(); x++)
                //{
                //    (*lines)[i] = (*s_array)[x];
                //}
                
                std::vector<Semantic *> * s_array = rm->analyze(textOnExpansion, false);

                std::vector<Semantic *>::iterator linesIter = lines->begin()+i;
                for (int iter = 0; iter < s_array->size(); iter++)
                    linesIter = lines->insert(linesIter, (*s_array)[iter]);
                
            }
            else
            {
                macroTable.back()->setText(Item);
            }
            
        }
        
        else
        {
            if (macroFound.size() != 0)
            {
                macroTable.back()->setText(Item);
            }
        }
    }

    if (macroFound.size() != 0)
    {
        std::cout << "Erro: macro n foi definida direito (FIM DO ARQUIVO)" << std::endl;
    }

    for (int i = 0; i < lines->size(); i++)
    {
        Semantic *Item = (*lines)[i];
        
        if (Item->getType() == Instruction::iMACRO)
        {
            macroFound.push_back(true);
        }
        if (Item->getType() == Instruction::iENDM)
        {
            macroFound.pop_back();
        }

        if (macroFound.size() == 0)
        {
            if (Item->getType() != Instruction::iMACROCALL)
            {
                outputResult += Item->getLine();
            }
        }
    }

    //Teste das macros tabeladas
    /*
    std::cout << "Total: " << macroTable.size() << std::endl << std::endl;
    for (int i = 0; i < (int) macroTable.size(); i++)
    {
        macroDef * item = macroTable[i];
        std::cout << item << std::endl;
    }
    */
    return outputResult;

}
