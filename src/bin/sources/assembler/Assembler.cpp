#include "Assembler.h"

#include "../analyzer/Semantic.h"
#include "../analyzer/RecognitionManager.h"
#include "../GlobalSettings.h"

#ifdef PRODUCTION_MODE
#include "../InterfaceBus.h"
#endif

#define File std::vector<Semantic *>

std::string Assembler::getOutput() {
    return output;
}

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

/**
 * TABELA OS SÍMBOLOS
 */
int Assembler::basicoAssemblerStep1()
{
    programCounter = 0;     //LC
    
    



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
                    return basicoAssemblerStep2();
                break;
defaultValue
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
    
    return 0;
}

//NECESSÁRIO PARA A ENTREGA 3
int Assembler::basicoAssemblerStep2()
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

    return 0;
}

int Assembler::assemble(int assemblerType)
{
    //Switch case com assemblerType
    //Métodos de assembler devem retornar flags de erro; tratar elas
    int flag = basicoAssemblerStep1();
    return 0;
}


//=====================================================================
//                            PREPROCESSOR
//=====================================================================

void Assembler::replaceAll(std::string& str, const std::string& from, const std::string& to) {

    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

int Assembler::macroExpandParams(MacroCall * macrocall, int k) {

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

    return preproccess(rm->analyze(input, false), k);

}

int Assembler::preproccess(std::vector<Semantic *> * lines, int k) {

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

            macroExpandParams(macroCall, k);

            continue;

        }

        std::cout << "c) line: " << item->getLine();

        output += item->getLine();

    }

    return k;

}

void Assembler::init(bool willExecute) {

    PRODUCTION(double startTime = InterfaceBus::getInstance().getMilliseconds())
    this->preproccess(lines, 0);
    PRODUCTION(double totalTime = (InterfaceBus::getInstance().getMilliseconds() - startTime) / 1000)

    TEST(std::cout << "============ INIT ==========" << std::endl)
    TEST(std::cout << output << std::endl)
    TEST(std::cout << "============ INIT ==========" << std::endl)

    if(willExecute) return;

    PRODUCTION(InterfaceBus::getInstance().dispatchMacroExpanded(output))
    PRODUCTION(InterfaceBus::getInstance().dispatchLog("Macros expandidas com sucesso!", LogStatus::SUCCESS))
    PRODUCTION(
        InterfaceBus::getInstance().dispatchLog(
            std::string("Tempo:    ") + std::to_string(totalTime) + std::string(" segundos"),
            LogStatus::INFO
    ))
}
