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

template <class T>
void Assembler::tableArithmeticInstructions(T *t)
{
    std::set<std::string> *set;

    if (set == nullptr)
        return;

    if (t->getExpression() != nullptr)
    { // instrucoes q o expression não for nullptr tem simbolo

        programCounter += 3;
        segmentCounter += 3;
        set = t->getSymbolSet();

        for (std::set<std::string>::iterator it = set->begin(); it != set->end(); it++)
        {
            // procurar p ver se já n tem o simbolo na tabela
            if (currentSegment->getSymbol(*it) == nullptr)
                currentSegment->setSymbol(new Symbol(*it, std::string("??")));
        }

        return;
    }

    programCounter += 2;
    segmentCounter += 2;
}

template <class T>
void Assembler::tableJumpsInstruction(T *t)
{
    programCounter += 3;
    segmentCounter += 3;

    std::set<std::string> *set;

    set = t->getSymbolSet();

    if (set == nullptr)
        return;

    for (std::set<std::string>::iterator it = set->begin(); it != set->end(); it++)
    {
        // procurar p ver se já n tem o simbolo na tabela
            if (currentSegment->getSymbol(*it) == nullptr)
                currentSegment->setSymbol(new Symbol(*it, std::string("??"), false, true));
    }
}

template <class T>
void Assembler::tableVarInstruction(T *t, bool isConst)
{
    if (!isConst)
    {
        segmentCounter += 2;
        programCounter += 2;
    }

    std::set<std::string> *set;

    set = t->getSymbolSet();

    if (set == nullptr)
        return;

    for (std::set<std::string>::iterator it = set->begin(); it != set->end(); it++)
    {
        // procurar p ver se já n tem o simbolo na tabela
        if (currentSegment->getSymbol(*it) == nullptr)
            currentSegment->setSymbol(new Symbol(*it, std::string("??"), true, false));
    }
}

/**
 * TABELA OS SÍMBOLOS
 */
int Assembler::basicoAssemblerStep1()
{
    programCounter = 0; // LC
    segmentCounter = 0;
    Instruction instruction;
    USint value;

    std::vector<Token *> *expression;
    Symbol *s;
    Semantic *line;
    bool inSegment = false;

    typedef struct PendingResolution {
        Symbol *symbol;
        SegmentDef *segment;
        Equ *semantic;
        PendingResolution(Symbol *symbol, SegmentDef *segment, Equ *semantic) :
            symbol(symbol), segment(segment), semantic(semantic)
        {}
    } PendingResolution;

    std::vector<PendingResolution *> dependencyMap;

    // for (File::iterator line = lines->begin(); line != lines->end(); ++line)
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

        if (!inSegment && instruction == Instruction::iSEGMENT)
        {
            Segment *segment = (Segment *)line;
            segmentTable[segment->getName()] = new SegmentDef(segment->getName(), programCounter);
            currentSegment = segmentTable.find(segment->getName())->second;

            // mov ax, Var1
            // mov ax, const1
            // mov ax, Var1+const1+var4+consty3

            segmentCounter = 0;
            continue;
        }

        switch (instruction)
        {
        // aritmeticas e logicas
        case Instruction::iADD:
            tableArithmeticInstructions<Add>((Add *)line);
            break;
        case Instruction::iSUB:
            tableArithmeticInstructions<Sub>((Sub *)line);
            break;
        case Instruction::iCMP:
            tableArithmeticInstructions<Cmp>((Cmp *)line);
            break;
        case Instruction::iOR:
            tableArithmeticInstructions<Or>((Or *)line);
            break;
        case Instruction::iAND:
            tableArithmeticInstructions<And>((And *)line);
            break;
        case Instruction::iXOR:
            tableArithmeticInstructions<Xor>((Xor *)line);
            break;

        // desvio
        case Instruction::iJMP:
            tableJumpsInstruction<Jmp>((Jmp *)line);
            break;
        case Instruction::iJE:
            tableJumpsInstruction<Je>((Je *)line);
            break;
        case Instruction::iJNZ:
            tableJumpsInstruction<Jnz>((Jnz *)line);
            break;
        case Instruction::iJZ:
            tableJumpsInstruction<Jz>((Jz *)line);
            break;
        case Instruction::iJP:
            tableJumpsInstruction<Jp>((Jp *)line);
            break;
        case Instruction::iCALL:
            tableJumpsInstruction<Call>((Call *)line);
            break;
        case Instruction::iINT:
            tableJumpsInstruction<Int>((Int *)line);
            break;

        // movimetação
        case Instruction::iMOV:

            // if(((Mov *) line)->getOperand1() != null )

            programCounter += 3;
            segmentCounter += 3;
            //
            break;

        case Instruction::iASSUME:
        {
            Assume *assume = (Assume *)line;

            if (assume->getSegmentRegister() == "cs" &&
                segmentTable.find(assume->getName()) != segmentTable.end())
            {
                assumedProgramSegment = segmentTable.find(assume->getName())->second;
            }
            else if (assume->getSegmentRegister() == "ds" &&
                     segmentTable.find(assume->getName()) != segmentTable.end())
            {
                assumedDataSegment = segmentTable.find(assume->getName())->second;
            }
            else if (assume->getSegmentRegister() == "ss" &&
                     segmentTable.find(assume->getName()) != segmentTable.end())
            {
                assumedStackSegment = segmentTable.find(assume->getName())->second;
            }
            break;
        }

        case Instruction::iORG: // error: jump to case label

            if(evaluate(((Org *) line)->getExpression(), &value) != nullptr) {
                programCounter += value;
                segmentCounter += value;
                break;
            }

            return ERROR;

        case Instruction::iDW:
        {
            Dw *dw = (Dw *)line;
            if (currentSegment->getSymbol(dw->getName()) == nullptr)
            {
                currentSegment->setSymbol(new Symbol(dw->getName(), std::string("??"), true, false));
            }

            tableVarInstruction<Dw>(dw, false);

            currentSegment->getSymbol(dw->getName())->value = std::to_string(segmentCounter);

            break;
        }

        case Instruction::iEQU:
        {
            Equ *equ = (Equ *)line;
            if (currentSegment->getSymbol(equ->getName()) == nullptr)
            {
                currentSegment->setSymbol(new Symbol(equ->getName(), std::string("??"), false, false));
            }

            tableVarInstruction<Equ>(equ, true);

            if (evaluate(equ->getExpression(), &value) != nullptr)
            {
                currentSegment->getSymbol(equ->getName())->value = std::to_string(value);
                break;
            }

            dependencyMap.emplace_back(new PendingResolution(currentSegment->getSymbol(equ->getName()), currentSegment, equ));
        /*
        ../src/bin/sources/assembler/Assembler.cpp:278:104: 
        error: no matching function for call to 
        ‘std::vector<Assembler::basicoAssemblerStep1()::PendingResolution*>::emplace_back(<brace-enclosed initializer list>)’
        278 |  dependencyMap.emplace_back({currentSegment->getSymbol(equ->getName()), currentSegment, equ});
        */                                                                                                    
            break;
        }
        case Instruction::iLABEL:
        {
            Label * label = (Label *) line;
            if (currentSegment->getSymbol(label->getName()) == nullptr)
            {
                currentSegment->setSymbol(new Symbol(label->getName(), std::string("??")));
            }

            currentSegment->getSymbol(label->getName())->value = segmentCounter;
            break;
        }
        case Instruction::iEND:
            // if (rotulo existe)
            // return SUCCESS;
            // else
            // return ERROR;
            break;

        case Instruction::iENDS:
            // Comecar catalogacao de variaveis dentro do segmento de codigo
            break;

        default:
            //
            break;
        }
    }

    PendingResolution * dep;
    bool symbolWasResolved = false;
    do
    {
        for (auto i = dependencyMap.begin(); i != dependencyMap.end(); i++)
        {
            /*
../src/bin/sources/assembler/Assembler.cpp:298:43: error: cannot bind non-const lvalue reference of type ‘__gnu_cxx::__normal_iterator<Assembler::basicoAssemblerStep1()::PendingResolution**, std::vector<Assembler::basicoAssemblerStep1()::PendingResolution*> >&’ to an rvalue of type ‘std::vector<Assembler::basicoAssemblerStep1()::PendingResolution*>::iterator’ {aka ‘__gnu_cxx::__normal_iterator<Assembler::basicoAssemblerStep1()::PendingResolution**, std::vector<Assembler::basicoAssemblerStep1()::PendingResolution*> >’}
  298 |         for (auto &i = dependencyMap.begin(); i != dependencyMap.end(); i++)
      |                        ~~~~~~~~~~~~~~~~~~~^~

            */            
            dep = *i;
            if (evaluate(dep->semantic->getExpression(), &value) != nullptr)
            {
                dep->symbol->value = std::to_string(value);
                delete dep;
                dependencyMap.erase(i);
                symbolWasResolved = true;
                i--;
            }
        }
        if (!symbolWasResolved)
        {
            break;
        }
    } while ((int)dependencyMap.size() > 0);

    return 0;
}

/*
    lida com o ExpressionEvaluator e retorna o vetor de bytes do valor resultante
*/
std::vector<byte> *Assembler::evaluate(Expression *expression, USint *valueHolder)
{
    ExpressionEvaluator *evaluator = new ExpressionEvaluator(expression, assumedProgramSegment, assumedDataSegment);
    /*
    ../src/bin/sources/assembler/Assembler.cpp:352:72: error: no matching function for call to ‘ExpressionEvaluator::ExpressionEvaluator(Expression*&)’
  352 |     ExpressionEvaluator *evaluator = new ExpressionEvaluator(expression);
      |                                                                        ^
    */
    if (evaluator->couldNotSymbolBeResolved())
    {
        delete evaluator;
        return nullptr;
    }
    USint value = evaluator->getValue();

    delete evaluator;

    if (valueHolder != nullptr)
        *valueHolder = value;

    return new std::vector<byte>({(value & 0xFF), (value >> 8)});
}

// produz o código de máquina associado ao código da instrução e operandos
template <class T>
std::vector<byte> * Assembler::generateAssembly(T *line)
{
    std::vector<byte> * expressValue, * opcode;

    // não depende de expressão
    if (line->getExpression() == nullptr)
    {
        return line->getOpCode();
    }

    // C.C. junta o opcode com o evaluate
    expressValue = evaluate(line->getExpression(), nullptr);
    
    opcode = line->getOpCode();

    for (int i = 0; expressValue != nullptr && i < expressValue->size(); i++)
    {
        std::cout << "#" << i << ":  " << (int) expressValue->at(i) << std::endl;
        opcode->push_back(expressValue->at(i));
    }
    

    return opcode;
}

void Assembler::GetSpecialOpcode(Semantic *line)
{
    // assemblyCode.push_back(((Div *)line)->getOpcode());
    std::vector<byte> * lineCode = line->getOpCode();
    for (int i = 0; lineCode != nullptr && i < lineCode->size(); i++)
    {
        assemblyCode.push_back(lineCode->at(i));
    }
}

/*
void GetAritmeticLogicOpcode(){
    lineCode = generateAssembly<Add>((Add *) line);
                if (lineCode == nullptr){
                    return ERROR;
                }else
                {
                    for(int i = 0; lineCode != nullptr && i < lineCode->size(); i++){
                        assemblyCode.push_back(lineCode->at(i));
                    }
                }
}
*/

// NECESSÁRIO PARA A ENTREGA 3
int Assembler::basicoAssemblerStep2()
{
    USint value;

    startProgram = 0;

    Semantic *line;
    Instruction instruction;

    programCounter = 0;
    segmentCounter = 0;

    std::string endLabel;

    Symbol *label;

    std::vector<byte> *lineCode;
    std::vector<byte> *expressionValue;

    bool inSegment = false;

    for (int i = 0; i < lines->size(); ++i)
    {
        line = lines->at(i);
        instruction = line->getType();

        if (!inSegment && instruction == Instruction::iSEGMENT)
        {
            Segment *segment = (Segment *)line;
            
            segmentCounter = 0;
            
            currentSegment = segmentTable.find(segment->getName())->second;
            
            inSegment = true;
            
            continue;
        }

        switch (instruction)
        {
        case Instruction::iEQU:
            break;
            // Nada

        case Instruction::iORG:

            lineCode = evaluate(((Org *)line)->getExpression(), &value); 
            /*
            ../src/bin/sources/assembler/Assembler.cpp:469:66: error: ‘value’ was not declared in this scope; did you mean ‘evaluate’?
  469 |             lineCode = evaluate(((Org *)line)->getExpression(), &value);
      |                                                                  ^~~~~
      |                                                                  evaluate
      */
            if (lineCode == nullptr)
            {
                return ERROR;
            }
            else
            {
                programCounter += value;
                segmentCounter += value;
            }

            break;

        case Instruction::iEND:
            endLabel = ((End *)line)->getName();
            label = currentSegment->getSymbol(endLabel);

            if (label == nullptr)
            {
                return ERROR;
            }
            else
            {
                startProgram = std::stoi(label->value);
            }

            return SUCCESS;
            break;

        case Instruction::iENDS:
            endLabel = ((EndS *)line)->getName(); 

            if (endLabel == currentSegment->getName())
            {
                return ERROR;
            }
            else
            {
                inSegment = false;
            }

            break;

        // casos especiais
        case Instruction::iDIV:
        case Instruction::iRET:
        case Instruction::iNOT:
        case Instruction::iMUL:
            GetSpecialOpcode(line);
            break;

        case Instruction::iMOV:
        {
            lineCode = line->getOpCode();

            Mov * mov = (Mov *) line;
            if (mov->getSymbolSet() == nullptr)
            {
                for (int i = 0; lineCode != nullptr && i < lineCode->size(); i++)
                {
                    assemblyCode.push_back(lineCode->at(i));
                }
            }
            else
            {
                if (mov->getExpression1() == nullptr && mov->isIndexed())
                {
                    // MOV AX, [SI] -- check
                    for (int i = 0; lineCode != nullptr && i < lineCode->size(); i++)
                    {
                        assemblyCode.push_back(lineCode->at(i));
                    }

                    // MOV AX,mem[SI] -- usar evalute -- check
                    expressionValue = evaluate(mov->getExpression2(), nullptr);
                    for (int i = 0; expressionValue != nullptr && i < expressionValue->size(); i++)
                    {
                        assemblyCode.push_back(expressionValue->at(i)); //??????
                    }
                }
                else if (mov->getExpression1() == nullptr)
                {
                    // MOV AX,cte -- ignorar por enquanto
                    // MOV AX, mem -- ignorar por enquanto
                }
                else if (mov->getExpression2() == nullptr && mov->isIndexed())
                {
                    // MOV mem[SI], AX -- check
                    for (int i = 0; lineCode != nullptr && i < lineCode->size(); i++)
                    {
                         assemblyCode.push_back(lineCode->at(i));
                    }
                    
                    expressionValue = evaluate(mov->getExpression1(), nullptr);
                    for (int i = 0; expressionValue != nullptr && i < expressionValue->size(); i++)
                    {
                        assemblyCode.push_back(expressionValue->at(i));
                    }
                }
                else if (mov->getExpression2() == nullptr)
                {
                    // MOV [SI], AX -- check
                    if (mov->getExpression1()->empty())
                    {
                        for (int i = 0; lineCode != nullptr && i < lineCode->size(); i++)
                        {
                            assemblyCode.push_back(lineCode->at(i));
                        }
                    }

                    // MOV mem, AX
                }
            }

            break;
        }
        // aritmeticas e logicas
        case Instruction::iADD:
            lineCode = generateAssembly<Add>((Add *)line);
            if (lineCode == nullptr)
            {
                return ERROR;
            }
            else
            {
                for (int i = 0; lineCode != nullptr && i < lineCode->size(); i++)
                {
                    assemblyCode.push_back(lineCode->at(i));
                }
            }
            break;
        case Instruction::iSUB:
            lineCode = generateAssembly<Sub>((Sub *)line);
            if (lineCode == nullptr)
            {
                return ERROR;
            }
            else
            {
                for (int i = 0; lineCode != nullptr && i < lineCode->size(); i++)
                {
                    assemblyCode.push_back(lineCode->at(i));
                }
            }
            break;
        case Instruction::iCMP:
            lineCode = generateAssembly<Cmp>((Cmp *)line);
            if (lineCode == nullptr)
            {
                return ERROR;
            }
            else
            {
                for (int i = 0; lineCode != nullptr && i < lineCode->size(); i++)
                {
                    assemblyCode.push_back(lineCode->at(i));
                }
            }
            break;
        case Instruction::iOR:
            lineCode = generateAssembly<Or>((Or *)line);
            if (lineCode == nullptr)
            {
                return ERROR;
            }
            else
            {
                for (int i = 0; lineCode != nullptr && i < lineCode->size(); i++)
                {
                    assemblyCode.push_back(lineCode->at(i));
                }
            }
            break;
        case Instruction::iAND:
            lineCode = generateAssembly<And>((And *)line);
            if (lineCode == nullptr)
            {
                return ERROR;
            }
            else
            {
                for (int i = 0; lineCode != nullptr && i < lineCode->size(); i++)
                {
                    assemblyCode.push_back(lineCode->at(i));
                }
            }
            break;
        case Instruction::iXOR:
            lineCode = generateAssembly<Xor>((Xor *)line);
            if (lineCode == nullptr)
            {
                return ERROR;
            }
            else
            {
                for (int i = 0; lineCode != nullptr && i < lineCode->size(); i++)
                {
                    assemblyCode.push_back(lineCode->at(i));
                }
            }
            break;

        // desvio
        case Instruction::iJMP:
            lineCode = generateAssembly<Jmp>((Jmp *)line);
            if (lineCode == nullptr)
            {
                return ERROR;
            }
            else
            {
                for (int i = 0; lineCode != nullptr && i < lineCode->size(); i++)
                {
                    assemblyCode.push_back(lineCode->at(i));
                }
            }
            break;
        case Instruction::iJE:
            lineCode = generateAssembly<Je>((Je *)line);
            if (lineCode == nullptr)
            {
                return ERROR;
            }
            else
            {
                for (int i = 0; lineCode != nullptr && i < lineCode->size(); i++)
                {
                    assemblyCode.push_back(lineCode->at(i));
                }
            }
            break;
        case Instruction::iJNZ:
            lineCode = generateAssembly<Jnz>((Jnz *)line);
            if (lineCode == nullptr)
            {
                return ERROR;
            }
            else
            {
                for (int i = 0; lineCode != nullptr && i < lineCode->size(); i++)
                {
                    assemblyCode.push_back(lineCode->at(i));
                }
            }
            break;
        case Instruction::iJZ:
            lineCode = generateAssembly<Jz>((Jz *)line);
            if (lineCode == nullptr)
            {
                return ERROR;
            }
            else
            {
                for (int i = 0; lineCode != nullptr && i < lineCode->size(); i++)
                {
                    assemblyCode.push_back(lineCode->at(i));
                }
            }
            break;
        case Instruction::iJP:
            lineCode = generateAssembly<Jp>((Jp *)line);
            if (lineCode == nullptr)
            {
                return ERROR;
            }
            else
            {
                for (int i = 0; lineCode != nullptr && i < lineCode->size(); i++)
                {
                    assemblyCode.push_back(lineCode->at(i));
                }
            }
            break;
        case Instruction::iCALL:
            lineCode = generateAssembly<Call>((Call *)line);
            if (lineCode == nullptr)
            {
                return ERROR;
            }
            else
            {
                for (int i = 0; lineCode != nullptr && i < lineCode->size(); i++)
                {
                    assemblyCode.push_back(lineCode->at(i));
                }
            }
            break;
        case Instruction::iINT:
            lineCode = generateAssembly<Int>((Int *)line);
            if (lineCode == nullptr)
            {
                return ERROR;
            }
            else
            {
                for (int i = 0; lineCode != nullptr && i < lineCode->size(); i++)
                {
                    assemblyCode.push_back(lineCode->at(i));
                }
            }
            break;
        default:
            break;
        }
    }

    return 0;
}

int Assembler::assemble(int assemblerType)
{
    // Switch case com assemblerType
    // Métodos de assembler devem retornar flags de erro; tratar elas
        std::cout << "Step 1" << std::endl;
    if(basicoAssemblerStep1() == SUCCESS) 
    {
        std::cout << "Step 2" << std::endl;
        if (basicoAssemblerStep2() == SUCCESS) 
        {
            std::cout << "FIM" << std::endl;
            TEST(std::cout << "Constução bem sucedida" << std::endl); 
            return 0;
        }
        TEST(std::cout << "ERRO (step 2)" << std::endl);
        return 1;
    } 
    TEST(std::cout << "ERRO (step 1)" << std::endl);
    
    return 1;
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

    std::vector<std::pair<std::string, std::string> *> *paramValue = macro->getParamValuePairs(macrocall);

    // ordena os pares (parametro_da_macro, valor) do maior para o menor
    std::sort(paramValue->begin(), paramValue->end(),
              [](std::pair<std::string, std::string> *pair1, std::pair<std::string, std::string> *pair2)
              {
                  return pair1->first.size() > pair2->first.size();
              });

    // itera sobre as linhas da macro (MACROCONTENT)
    for (int i = 0; i < (int)macro->getText()->size(); i++)
    {

        aux = Utils::toLowerCase(macro->getText()->at(i)->getLine());
        macrocontent = "";

        // insere espaços do lado dos caracteres especiais
        for (int j = 0; j < aux.size(); j++)
        {
            char c = aux[j];
            if (!Utils::isSpecialCharactere(c))
            {
                macrocontent += c;
                continue;
            }
            macrocontent += std::string(" ") + c + std::string(" ");
        }

        std::vector<std::string> *macrocontentPieces = Utils::split(macrocontent, ' ');

        macrocontent = "";

        // para cada linha, itera sobre os pedaços dessa linha
        for (int j = 0; j < (int)macrocontentPieces->size(); j++)
        {
            if (Utils::isSpecialCharactere(macrocontentPieces->at(j)))
                continue;

            // tenta substituir o primeiro parametro que bater pelo valor correspondente
            for (int p = 0; p < (int)paramValue->size(); p++)
            {
                aux = macrocontentPieces->at(p);
                aux = Utils::replaceAll(aux, paramValue->at(p)->first, paramValue->at(p)->second);

                if (aux != macrocontentPieces->at(p))
                    break;
            }

            macrocontent += aux;

            TEST(std::cout << "macrocontent:  " << macrocontent << std::endl);
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

    PRODUCTION(double startTime = InterfaceBus::getInstance().getMilliseconds());
    this->preproccess(lines, 0);
    PRODUCTION(double totalTime = (InterfaceBus::getInstance().getMilliseconds() - startTime) / 1000);

    TEST(std::cout << "============ INIT ==========" << std::endl);
    TEST(std::cout << output << std::endl);
    TEST(std::cout << "============ INIT ==========" << std::endl);

    if (willExecute)
        return;

    PRODUCTION(InterfaceBus::getInstance().dispatchMacroExpanded(output));
    PRODUCTION(InterfaceBus::getInstance().dispatchLog("Macros expandidas com sucesso!", LogStatus::SUCCESS));
    PRODUCTION(
        InterfaceBus::getInstance().dispatchLog(
            std::string("Tempo:    ") + std::to_string(totalTime) + std::string(" segundos"),
            LogStatus::INFO));
}
/*

*/

std::vector<byte> * Assembler::getAssemblyCode() {
    return &assemblyCode;
}