#include "Assembler.h"

#include "../analyzer/Semantic.h"
#include "../analyzer/RecognitionManager.h"
#include "../GlobalSettings.h"

#ifdef PRODUCTION_MODE
#include "../InterfaceBus.h"
#endif

#define File std::vector<Semantic *>

std::string Assembler::getOutput()
{
    return output;
}

Assembler::Assembler(std::vector<Semantic *> *lines) : lines(lines)
{
}

/** Coisas de Semantic que precisamos pegar:
 * getOperands - não entendi como funciona, talvez não seja o que precisamos
 * getLabel - Só tem o getName de uma subclasse Semantic Label
 * a subclasse pra pseudoinstrução ORG - não está implementada
 * (talvez tenha mais, mas por enquanto foi o que notamos)
 ** Coisas de Semantic prontas:
 * getType - retorna um enum, com base nele podemos pegar o código de máquina referente
 */

void Assembler::generateAssembly(std::vector<unsigned char> bytecode)
{
    //Concatenar bytecode no final de assemblyCode
    return;
}

template <class T>
void Assembler::tableArithmeticInstructions(T *t)
{
    Symbol *s;
    std::set<std::string> * set;

    if (t->getExpression() != nullptr){     //instrucoes q o expression não for nullptr tem simbolo

        programCounter += 3;
        set = t->getSymbolSet();

        for(std::set<std::string>::iterator it = set.begin(); it != set.end()) {
            s = new Symbol(*it,"??");

            //procurar p ver se já n tem o simbolo na tabela
            if(symbolTable.find(*it) != symbolTable.end())
                symbolTable.insert(s->name, s);
        }

        return;

    }

    programCounter += 2;
}

template <class T>
void Assembler::tableJumpsInstruction(T *t) 
{
    programCounter += 3;
    
    std::set<std::string> * set;

    set = t->getSymbolSet();

    for(std::set<std::string>::iterator it = set.begin(); it != set.end()) {
        s = new Symbol(*it,"??");

        //procurar p ver se já n tem o simbolo na tabela
        if(symbolTable.find(*it) != symbolTable.end())
            symbolTable.insert(s->name, s);
    }
}

/**
 * TABELA OS SÍMBOLOS
 */
int Assembler::basicoAssemblerStep1()
{
    programCounter = 0; // LC
    Instruction instruction;
    std::vector<Token *> * expression;
    Symbol *s;
    Semantic * line;

    //for (File::iterator line = lines->begin(); line != lines->end(); ++line)
    for (int i = 0; i < lines->size(); ++i)
    {
        std::vector<Token *> *expression;
        line = lines->at(i);
        // expressão = std::vector<Token *> *

        // aritmeticos exceto DIV e MUL
        // logicos exceto NOT
        // desvio exceto RET
        // declaração de procedimento
        // movimentação
        // DW, EQU, ORG

        instruction = line->getType();
        switch (instruction)
        {
            //aritmeticas e logicas
            case Instruction::iADD:
                tableArithmeticInstructions<Add>((Add *) line);
                break;
            case Instruction::iSUB:
                tableArithmeticInstructions<Sub>((Sub *) line);
                break;
            case Instruction::iCMP:
                tableArithmeticInstructions<Cmp>((Cmp *) line);
                break;
            case Instruction::iOR:
                tableArithmeticInstructions<Or>((Or *) line);
                break;
            case Instruction::iAND:
                tableArithmeticInstructions<And>((And *) line);
                break;
            case Instruction::iXOR:
                tableArithmeticInstructions<Xor>((Xor *) line);
                break;

            //desvio
            case Instruction::iJMP:
                tableJumpsInstruction<Jmp>((Jmp *) line);
                break;
            case Instruction::iJE:
                tableJumpsInstruction<Je>((Je *) line);
                break;
            case Instruction::iJNZ:
                tableJumpsInstruction<Jnz>((Jnz *) line);
                break;
            case Instruction::iJZ:
                tableJumpsInstruction<Jz>((Jz *) line);
                break;
            case Instruction::iJP:
                tableJumpsInstruction<Jp>((Jp *) line);
                break;
            case Instruction::iCALL:
                tableJumpsInstruction<Call>((Call *) line);
                break;
            case Instruction::iINT:
                tableJumpsInstruction<Int>((Int *) line);
                break;
            
            //movimetação
            case Instruction::iMOV:

                programCounter += 3;
                //
                break;

            case Instruction::iORG:

                expression = ((Org *) line)->getExpression();
                //programCounter = line.getValue();

                break;

            case Instruction::iDW:
/*
                if(line){
                    if( symbolTable.find(line) != symbolTable.end()){
                        // coloca rótulo na TS
                        symbolTable.insert(line->getLine(), line->getType());
                        programCounter += 2;
                    }else{
                        programCounter =  int( expression = ((Dw *) line)->getLength() );
                    }
                }
// if Instrucao contém rotulo
// then if rotulo não está em TS
// coloca rótulo na TS com PC
//		pc += 2
  // else if Instrucao é instrucao de máquina (pesquisar na TIM)
	//then pc += Tamanho da inst
*/



                //s = new Symbol(line->getName(), , );

                //symbolTable.insert(s);
                break;

            case Instruction::iEQU:
                //
                break;
            
            case Instruction::iEND:
                //if (rotulo existe)
                  //basicoAssemblerStep2();
                //else
                  //std::cout << ("ERROR"); 
                break;

            
            default:
                //
                break;
        }
    }

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

// NECESSÁRIO PARA A ENTREGA 3
int Assembler::basicoAssemblerStep2()
{
    Semantic * line;
    Instruction instruction;
    programCounter = 0;
    
    for (int i = 0; i < lines->size(); ++i)
    {

        instruction = line->getType();
        switch (instruction)
        {
            case Instruction::iEQU:
            break;
            //
            case Instruction::iORG:
            //std::vector<Token *> * getExpression() const;
            //std::set<std::string> * getSymbolSet();

                //programCounter = processarExpressao;
 
            break;

            case Instruction::iEND:

            break;

            //default:
            //DC e DS tem no Z808? C.c ignorar

            //aritmeticas e logicas
            case Instruction::iADD:
                //generateAssembly(processarExpressao<Add>((Add *) line));
            break;
            case Instruction::iSUB:
                //generateAssembly(processarExpressao<Sub>((Sub *) line));
            break;
            case Instruction::iCMP:
                //generateAssembly(processarExpressao<Cmp>((Cmp *) line));
            break;
            case Instruction::iOR:
                //generateAssembly(processarExpressao<Or>(Or *) line));
            break;
            case Instruction::iAND:
                //generateAssembly(processarExpressao<And>((And *) line));
            break;
            case Instruction::iXOR:
                //generateAssembly(processarExpressao<Xor>((Xor *) line));
                break;

            //desvio
            case Instruction::iJMP:
                //tableJumpsInstruction<Jmp>((Jmp *) line);
                
                //generateAssembly(processarExpressao<Jmp>((Jmp *) line));
                break;
            case Instruction::iJE:
                 //generateAssembly(processarExpressao<Je>((Je *) line));
                break;
            case Instruction::iJNZ:
                 //generateAssembly(processarExpressao<Jnz>((Jnz *) line));
                break;
            case Instruction::iJZ:
                 //generateAssembly(processarExpressao<Jz>((Jz *) line));
                break;
            case Instruction::iJP:
                 //generateAssembly(processarExpressao<Jp>((Jp *) line));
                break;
            case Instruction::iCALL:
                 //generateAssembly(processarExpressao<Call>((Call *) line));
                break;
            case Instruction::iINT:
                tableJumpsInstruction<Int>((Int *) line);
                break;
        }
    }

    /*

    programCounter = 0; // LC
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
                switch(instruction) {
                    case Instruction::iADD:
                }
                //if(opcode == 'DC' || opcode == 'DS'){
                    //entry = findtable(POT, opcode);
                //}else{
                    //entry = findtable(MOT, opcode);
                //}

                //length = getinstructionsize(entry, operand);
                //code = assembleByteCode(line);
                //code = gerenatemachinecode(entry, operand);

                //Concatena os vetores code e this->assemblyCode
                //assemblemachinecode(locationCounter, code);    //Usa this->assemblyCode, std::vector<unsigned char>
                //locationCounter = locationCounter + length;

            break;
    }
    */

    return 0;
}

int Assembler::assemble(int assemblerType)
{
    // Switch case com assemblerType
    // Métodos de assembler devem retornar flags de erro; tratar elas
    
    int flag = basicoAssemblerStep1(); 
    return 0;
}

//=====================================================================
//                            PREPROCESSOR
//=====================================================================

int Assembler::macroExpandParams(MacroCall *macrocall, int k)
{

    std::cout << "============ EXPANDS ==========" << std::endl;

    RecognitionManager *rm = new RecognitionManager();

    std::string input = "", macrocontent, aux;

    MacroDef *macro = macroTable.find(macrocall->getName())->second;

    std::vector<std::pair<std::string, std::string> *> * paramValue = macro->getParamValuePairs(macrocall);

    // ordena os pares (parametro_da_macro, valor) do maior para o menor
    std::sort(paramValue->begin(), paramValue->end(), 
        [](std::pair<std::string, std::string> * pair1, std::pair<std::string, std::string> * pair2) {
            return pair1->first.size() > pair2->first.size();
    });

    // itera sobre as linhas da macro (MACROCONTENT)
    for (int i = 0; i < (int)macro->getText()->size(); i++)
    {

        aux = Utils::toLowerCase(macro->getText()->at(i)->getLine());
        macrocontent = "";

        // insere espaços do lado dos caracteres especiais
        for(int j = 0; j < aux.size(); j++) {
            char c = aux[j];
            if(!Utils::isSpecialCharactere(c)) {
                macrocontent += c;
                continue;
            }
            macrocontent += std::string(" ") + c + std::string(" ");
        }

        std::vector<std::string> * macrocontentPieces = Utils::split(macrocontent, ' ');

        macrocontent = "";
        
        // para cada linha, itera sobre os pedaços dessa linha
        for (int j = 0; j < (int) macrocontentPieces->size(); j++)
        {
            if(Utils::isSpecialCharactere(macrocontentPieces->at(j))) continue;


            // tenta substituir o primeiro parametro que bater pelo valor correspondente
            for (int p = 0; p < (int) paramValue->size(); p++)
            {
                aux = macrocontentPieces->at(p);
                aux = Utils::replaceAll(aux, paramValue->at(p)->first, paramValue->at(p)->second);

                if(aux != macrocontentPieces->at(p)) break;

            }

            macrocontent += aux;

            TEST(std::cout << "macrocontent:  " << macrocontent << std::endl)

        }
        

        input += macrocontent;
    }

    return preproccess(rm->analyze(input, false), k);
}

int Assembler::preproccess(std::vector<Semantic *> *lines, int k)
{

    std::vector<bool> macroFound; // Pilha de controle das declaracoes de macro

    std::cout << "============ PREPROCESS ==========" << std::endl;

    // tabela
    for (int i = 0; i < (int)lines->size(); i++, k++)
    {

        Semantic *item = lines->at(i);

        if (item->getType() == Instruction::iENDM)
        {

            std::cout << "t) fim de macro: " << item->getLine();

            if (macroFound.size() == 0)
            {
                // Erro
                std::cout << "Erro: macro mal declarada.";
            }

            macroList.back()->setText(item);
            macroFound.pop_back();

            // lines->erase(lines->begin()+i);
            // i--;

            continue;
        }

        if (item->getType() == Instruction::iMACRO)
        {

            std::cout << "t) macro: " << item->getLine();

            Macro *itemMacro = (Macro *)item;
            MacroDef *md = new MacroDef(itemMacro->getName(), *(itemMacro->getParams()));
            macroTable[itemMacro->getName()] = md;
            macroList.emplace_back(md);
            macroFound.emplace_back(true);

            // lines->erase(lines->begin()+i);
            // i--;

            continue;
        }

        // std::cout << "t) line: " << item->getLine();

        if (macroFound.size() > 0)
        {
            std::cout << "t) macrocontent: " << item->getLine();
            macroList.back()->setText(item);
            // lines->erase(lines->begin()+i);
            // i--;
        }
    }

    // expande
    for (int i = 0; i < (int)lines->size(); i++)
    {

        Semantic *item = lines->at(i);

        if (item->getType() == Instruction::iMACRO || item->getType() == Instruction::iENDM || item->getType() == Instruction::iMACROCONTENT)
        {
            continue;
        }

        if (item->getType() == Instruction::iMACROCALL)
        {

            std::cout << "c) call: " << item->getLine();

            MacroCall *macroCall = (MacroCall *)item;

            macroExpandParams(macroCall, k);

            continue;
        }

        std::cout << "c) line: " << item->getLine();

        output += item->getLine();
    }

    return k;
}

void Assembler::init(bool willExecute)
{

    PRODUCTION(double startTime = InterfaceBus::getInstance().getMilliseconds())
    this->preproccess(lines, 0);
    PRODUCTION(double totalTime = (InterfaceBus::getInstance().getMilliseconds() - startTime) / 1000)

    TEST(std::cout << "============ INIT ==========" << std::endl)
    TEST(std::cout << output << std::endl)
    TEST(std::cout << "============ INIT ==========" << std::endl)

    if (willExecute)
        return;

    PRODUCTION(InterfaceBus::getInstance().dispatchMacroExpanded(output))
    PRODUCTION(InterfaceBus::getInstance().dispatchLog("Macros expandidas com sucesso!", LogStatus::SUCCESS))
    PRODUCTION(
        InterfaceBus::getInstance().dispatchLog(
            std::string("Tempo:    ") + std::to_string(totalTime) + std::string(" segundos"),
            LogStatus::INFO))
}
