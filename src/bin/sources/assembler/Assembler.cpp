#include "Assembler.h"

#include "../analyzer/Semantic.h"
#include "../analyzer/RecognitionManager.h"
#include "../InterfaceBus.h"

#define File std::vector<Semantic *>

Assembler::Assembler(std::vector<Semantic *> * lines) :
    lines(lines)
{}

/** Coisas de Semantic que precisamos pegar:
 * getOperands - não entendi como funciona, talvez não seja o que precisamos
 * getLabel - Só tem o getName de uma subclasse Semantic Label
 * a subclasse pra pseudoinstrução ORG - não está implementada
 * (talvez tenha mais, mas por enquanto foi o que notamos)
 ** Coisas de Semantic prontas:
 * getType - retorna um enum, com base nele podemos pegar o código de máquina referente
 */

std::vector<unsigned char> Assembler::assembleByteCode(Semantic * line)
{
    std::vector<unsigned char> bytecode;
    //bytecode.push_back(bytecode(line->getType()));
    //bytecode.push_back(bytecode(line->getOperands()));
    return bytecode;
}

void Assembler::assembleStep2()
{
    /*

    int locationCounter = 0;
    int startAddress;
    Instruction instruction;

    for(line = this->lines->begin(); line != this->lines->end(); ++line)
    {
        instruction = line->getType();
        //operand = line->getOperand();     //pegar depois

        switch(instruction) {
            case Instruction::iEQU:
                // NADA
            break;

            case Instruction::iORG:
                //LC = GETOPERANDVALUE (operand)
            break;

            case Instruction::iEND:
                //startAddress = GETOPERANDVALUE(operand)
                //ASSEMBLECLOSINGCODE(startAdress);
            break;

            default:
                //if(opcode == 'DC' || opcode == 'DS'){
                    //entry = findtable(POT, opcode);
                //}else{
                    //entry = findtable(MOT, opcode);
                //}

                //length = getinstructionsize(entry, operand);
                //code = assembleByteCode(line);
                //code = gerenatemachinecode(entry, operand);

                //Concatena os vetores code e this->assembleCode
                //assemblemachinecode(locationCounter, code);    //Usa this->assembleCode, std::vector<unsigned char>
                //locationCounter = locationCounter + length;

            break;
    }
    */

    return;
}

void Assembler::assembleStep1()
{
    /*

    int locationCounter = 0;
    int label;                      //#### template ####
    int opcode;                     //#### template ####
    int operand;                    //#### template ####
    int* entry = NULL;              //#### template ####
    int value, length;
    
    //file == *this->lines
    for(File::iterator line = this->lines->begin(); line != this->lines->end(); ++line)
    {
        //label = line->getLabel();         //falta pegar da linha
        opcode = line->getType();
        //operand = line->getOperand();     //pegar depois

        // ** entry <- FINDTABLE(POT, opcode) **
        //entry = tabela_de_pseudoinstrucoes;

        if(entry != NULL)   //Se eh pseudoinstrucao
        {
            switch (opcode) 
            {
                case 'ORG':
                    // ** LC <-GETOPERANDVALUE (operand) ** 
                    //locationCounter = line->getValue();
                break;

                case 'END':
                    assembleStep2();
                break;

                case default:
                    if(opcode == 'EQU')
                    {
                        // ** value <- GETOPERANDVALUE (operand) ** 
                        //value = (operand);
                        length = 0;
                    }else
                    {
                        value = locationCounter;
                        //lenght(entry, operand);
                    }

                    if(label != NULL) 
                    {
                        // ** INSERTTABLE(ST, label, value) ** 
                        //(ST, label, value); //usar this->symbolTable (unordered_map) para ST
                        locationCounter = locationCounter + lenght;
                    }
                break;

            }
        }
        else
        {
            // ** entry <- FINDTABLE(MOT, opcode) **
            //entry = tabela_de_instrucoes_maquina;

            if(entry != NULL){
                // ** length <- GETINSTRUCTIONSZE(entry, operand) **
                length = (entry, operand);
                
                if(label != NULL)
                    //INSERTTABLE (ST, label, LC)
                    //(ST, label, LC);        //usar this->symbolTable (unordered_map) para ST
                    
                locationCounter = locationCounter + lenght;
            }
            else 
            {
                //error message, invalid opcode     
            }
        }
        
    }

    */
    
    return;
}

std::vector<unsigned char> Assembler::assemble(int assemblerType)
{
    assembleStep1();
    return assembleCode;
}


//=====================================================================
//                            PREPROCESSOR
//=====================================================================

std::string Assembler::macroExpandParams(std::vector<Symbol *> params, MacroDef * macroThis) {

    /**Ja definido em Assembler.h:
    *       std::vector<macroDef *> macroTable;
    *       std::vector<label *> labelTable;
    */

   std::vector<Semantic *> macroLines = *(macroThis->getText());

   std::string lineThis;
   std::string outputText = "";

   std::vector<bool> macroFound;

   int antes, depois;

   for (int i = 0; i < (int) macroLines.size(); i++)
   {

        if(macroLines[i]->getType() == Instruction::iMACRO)     //Tabelar a macro
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

                for (int iter = 0; iter < (int) params.size(); iter++)
                {
                    //antes = ( lineThis.find(params[iter]->name) ) - 1;
                    //depois = ( lineThis.find(params[iter]->name) ) + 1;

                    if(antes >= -1 && depois <= (int) lineThis.size()+1)       // para validar que é uma posição válida
                    {
                        if ((lineThis[antes] == ' ' || lineThis[antes] == ',') && (lineThis[depois] == ' ' || lineThis[depois] == ',' || lineThis[depois] == '\n')){
                            //lineThis.replace(lineThis.find(params[iter]->name), params[iter]->name.size(), params[iter]->valor);
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

    MacroDef * macro = macroTable.find(macrocall->getName())->second;

    for(int i = 0; i < (int) macro->getText()->size(); i++) {

        hold = macro->getText()->at(i)->getLine();

        std::transform(hold.begin(), hold.end(), hold.begin(),
            [](unsigned char c){
                return std::tolower(c);
        });

        for(int j = 0; j < (int) macro->getArgs().size(); j++) {

            std::string param = "";

            for(int p = 0; p < (int) macrocall->getParams()->at(j)->size(); p++) {
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


            macroList.back()->setText(item);
            macroFound.pop_back();

            //lines->erase(lines->begin()+i);
            //i--;

            continue;

        }

        if(item->getType() == Instruction::iMACRO) {

            std::cout << "t) macro: " << item->getLine();

            Macro * itemMacro = (Macro *) item;
            MacroDef * md = new MacroDef(itemMacro->getName(), *(itemMacro->getParams()));
            macroTable[itemMacro->getName()] = md;
            macroList.emplace_back(md);
            macroFound.emplace_back(true);

            //lines->erase(lines->begin()+i);
            //i--;

            continue;
        }

        //std::cout << "t) line: " << item->getLine();

        if(macroFound.size() > 0) {
            std::cout << "t) macrocontent: " << item->getLine();
            macroList.back()->setText(item);
            //lines->erase(lines->begin()+i);
            //i--;
        }

    }

    // expande
    for(int i = 0; i < (int) lines->size(); i++) {

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

void Assembler::init(bool willExecute) {

    double startTime = InterfaceBus::getInstance().getMilliseconds();
    this->preproccessDoDaniel(lines, 0);
    double totalTime = (InterfaceBus::getInstance().getMilliseconds() - startTime) / 1000;

    std::cout << "============ INIT ==========" << std::endl;
    std::cout << output << std::endl;
    std::cout << "============ INIT ==========" << std::endl;

    if(willExecute) return;

    InterfaceBus::getInstance().dispatchMacroExpanded(output);
    InterfaceBus::getInstance().dispatchLog("Macros expandidas com sucesso!", LogStatus::SUCCESS);
    InterfaceBus::getInstance().dispatchLog(
        std::string("Tempo:    ") + std::to_string(totalTime) + std::string(" segundos"),
        LogStatus::INFO
    );
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
                macroList.push_back(new MacroDef(itemMacro->getName(), *(itemMacro->getParams())));
                macroFound.push_back(true);
            }
            else
            {
                macroList.back()->setText(Item);
                macroFound.push_back(true);
            }
        }

        else if (Item->getType() == iENDM) {  //eu acho que ele não ta reconhecendo o iENDM como fim de macro

            if (macroFound.size() == 0){
                // Erro
                std::cout << "Erro: macro mal declarada.";
                break;
            } else {

                macroList.back()->setText(Item);
                macroFound.pop_back();

            }

        }
        else if (Item->getType() == Instruction::iMACROCALL){

            if (macroFound.size() == 0) //Nao esta no processo de tabelamento
            {                           //Inicia passo 1; salvar os parametros da chamada
                bool macroExists = false;

                MacroCall * itemMacroCall = (MacroCall *)((*lines)[i]);

                MacroDef *macroThis;    //Macro que estava na tabela

                for (std::vector<MacroDef*>::iterator it = macroList.begin(); it != macroList.end(); it++)   //Procura macro na tabela
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

                std::vector<Symbol *> params;

                for (int iter = 0; iter < (int) macroThis->getArgs().size(); iter++)   //Criando a lista de parâmetros
                {
                    //params.push_back(new Label);
                    //params.back()->name = macroThis->getArgs()[iter];
                    for (int iterToken = 0; iterToken < (int) ((MacroCall*) Item)->getParams()->at(iter)->size(); iterToken++)
                    {
                        std::string tokenResult;

                        //params.back()->valor = tokenResult;
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

                //std::vector<Semantic *> * s_array = rm->analyze(textOnExpansion, false);
                //std::vector<Semantic *> * prior = new std::vector(lines->begin(), lines->begin()+i-1);
                //std::vector<Semantic *> * after = new std::vector(lines->begin()+i+1, lines->end());
                //std::vector<Semantic *> * tempDelete = lines;
                //delete tempDelete;
                //std::copy(prior->begin(),prior->end(),lines->begin());
                //std::copy(s_array->begin(),s_array->end(),lines->end());
                //std::copy(after->begin(),after->end(),lines->end());

                //for(int x=0; x < (*s_array).size(); x++)
                //{
                //    (*lines)[i] = (*s_array)[x];
                //}

                std::vector<Semantic *> * s_array = rm->analyze(textOnExpansion, false);

                std::vector<Semantic *>::iterator linesIter = lines->begin()+i;
                for (int iter = 0; iter < (int) s_array->size(); iter++)
                    linesIter = lines->insert(linesIter, (*s_array)[iter]);
                lines->erase(lines->begin()+i);

            }
            else
            {
                macroList.back()->setText(Item);
            }

        }
        else
        {
            if (macroFound.size() != 0)
            {
                macroList.back()->setText(Item);
            }
        }
    }

    if (macroFound.size() != 0)
    {
        std::cout << "Erro: macro n foi definida direito (FIM DO ARQUIVO)" << std::endl;
    }

    for (int i = 0; i < (int) lines->size(); i++)
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

    //std::cout << "Total: " << macroTable.size() << std::endl << std::endl;
    //for (int i = 0; i < (int) macroTable.size(); i++)
    //{
    //    macroDef * item = macroTable[i];
    //    std::cout << item << std::endl;
    //}

    return outputResult;

}
