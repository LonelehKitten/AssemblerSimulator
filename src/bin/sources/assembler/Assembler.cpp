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

   std::vector<Semantic *> macroLines = *(macroThis->getText());

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

void Assembler::replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

int Assembler::macroExpandParamsDoDaniel(MacroCall * macrocall, int k) {

    std::cout << "============ EXPANDS ==========" << std::endl;
    
    RecognitionManager * rm = new RecognitionManager();
    
    std::string input = "", hold;

    macroDef * macro = macroMap.find(macrocall->getName())->second;

    for(int i = 0; i < macro->getText()->size(); i++) {

        hold = macro->getText()->at(i)->getLine();

        std::transform(hold.begin(), hold.end(), hold.begin(),
            [](unsigned char c){
                return std::tolower(c);
        });

        for(int j = 0; j < macro->getArgs().size(); j++) {

            std::string param = "";

            for(int p = 0; p < macrocall->getParams()->at(j)->size(); p++) {
                param += macrocall->getParams()->at(j)->at(p)->getToken();
            }

            std::cout << "param:  " << param << std::endl;

            replaceAll(hold, macro->getArgs()[j], param);
        }

        input += hold;
    }

    return preproccessDoDaniel(rm->analyze(input, false), k);

}

int Assembler::preproccessDoDaniel (std::vector<Semantic *> * lines, int k) {

    std::vector<bool> macroFound;   //Pilha de controle das declaracoes de macro

    std::cout << "============ PREPROCESS ==========" << std::endl;


    // tabela
    for(int i = 0; i < (int) lines->size(); i++, k++) {

        Semantic * item = lines->at(i);


        if(item->getType() == Instruction::iENDM) {

            std::cout << "t) fim de macro: " << item->getLine();

            if (macroFound.size() == 0){
                // Erro
                std::cout << "Erro: macro mal declarada.";
            }

            
            macroTable.back()->setText(item);
            macroFound.pop_back();

            //lines->erase(lines->begin()+i);
            //i--;

            continue;

        }

        if(item->getType() == Instruction::iMACRO) {

            std::cout << "t) macro: " << item->getLine();
            
            Macro * itemMacro = (Macro *) item;
            macroDef * md = new macroDef(itemMacro->getName(), *(itemMacro->getParams()));
            macroMap[itemMacro->getName()] = md;
            macroTable.emplace_back(md);
            macroFound.emplace_back(true);

            //lines->erase(lines->begin()+i);
            //i--;

            continue;
        }

        //std::cout << "t) line: " << item->getLine();

        if(macroFound.size() > 0) {
            std::cout << "t) macrocontent: " << item->getLine();
            macroTable.back()->setText(item);
            //lines->erase(lines->begin()+i);
            //i--;
        }

    }

    // expande
    for(int i = 0; i < lines->size(); i++) {

        Semantic * item = lines->at(i);


        if(item->getType() == Instruction::iMACRO || item->getType() == Instruction::iENDM || item->getType() == Instruction::iMACROCONTENT) {
            continue;
        }

        if(item->getType() == Instruction::iMACROCALL) {

            std::cout << "c) call: " << item->getLine();

            MacroCall * macroCall = (MacroCall *) item;

            macroExpandParamsDoDaniel(macroCall, k);

            continue;

        }

        std::cout << "c) line: " << item->getLine();

        output += item->getLine();

    }

    return k;

}

std::string Assembler::init() {

    //std::string output = "";
    this->preproccessDoDaniel(lines, 0);

    std::cout << "============ INIT ==========" << std::endl;
    std::cout << output << std::endl;
    std::cout << "============ INIT ==========" << std::endl;

    return output;
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
        std::cout << "-> " << Item->getLine() << std::endl;

        if (Item->getType() == iMACRO) {          //Definição de macro
        
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

        else if (Item->getType() == iENDM) {  //eu acho que ele não ta reconhecendo o iENDM como fim de macro
            
            if (macroFound.size() == 0){
                // Erro
                std::cout << "Erro: macro mal declarada.";
                break;
            } else {

                macroTable.back()->setText(Item);
                macroFound.pop_back();

            }

        }
        else if (Item->getType() == Instruction::iMACROCALL){
            
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
                lines->erase(lines->begin()+i);
                
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
