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

Assembler::Assembler(std::vector<Semantic *> *lines) :
    lines(lines),
    currentSegment(nullptr), 
    assumedProgramSegment(nullptr),
    assumedDataSegment(nullptr),
    assumedStackSegment(nullptr),
    output(""),
    lineCounter(0),
    programCounter(0),
    segmentCounter(0),
    startProgram(0), 
    assemblerError(0)
{
}

long Assembler::getStartProgram()
{
    return startProgram;
}

long Assembler::getStartSegment()
{
    if (currentSegment != nullptr)
        return std::stoi(currentSegment->value);
    return 0;
}

template <class T>
void Assembler::tableArithmeticInstructions(T *t)
{
    std::set<std::string> *set;

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
    /*
../src/bin/sources/assembler/Assembler.cpp:57:38: error: ‘class Int’ has no member named ‘getLabel’
   57 |     if (currentSegment->getSymbol(t->getLabel()) == nullptr)
   ⠦ Building module: AssemblerSimulator, Completed: 0../src/bin/sources/assembler/Assembler.cpp:58:53: error: ‘class Int’ has no member named ‘getLabel’
   58 |             currentSegment->setSymbol(new Symbol(t->getLabel(), std::string("??"), false, true));
    */
    if (currentSegment->getSymbol(t->getLabel()) == nullptr)
            currentSegment->setSymbol(new Symbol(t->getLabel(), std::string("??"), false, true));
}

template <class T>
void Assembler::tableIntInstruction(T *t)
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

void showLog(Instruction instruction, std::string line) {
    switch (instruction) {
        case Instruction::iADD: std::cout << "iADD "+line << std::endl; break;
        case Instruction::iASSUME: std::cout << "iASSUME "+line << std::endl; break;
        case Instruction::iSEGMENT: std::cout << "iSEGMENT "+line << std::endl; break;
        case Instruction::iMOV: std::cout << "iMOV "+line << std::endl; break;
        case Instruction::iJNZ: std::cout << "iJNZ "+line << std::endl; break;
        case Instruction::iENDS: std::cout << "iENDS " << std::endl; break;
        case Instruction::iEND: std::cout << "iEND "+line << std::endl; break;
        case Instruction::iEQU: std::cout << "iEQU "+line << std::endl; break;
        case Instruction::iDW: std::cout << "iDW "+line << std::endl; break;
        case Instruction::iLABEL: std::cout << "iLABEL "+line << std::endl; break;
        case Instruction::iSUB: std::cout << "iSUB "+line << std::endl; break;
        default: std::cout << std::to_string(instruction)+line << std::endl; break;
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

    bool isEnd = false;

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
    for (int i = 0; !isEnd && i < lines->size(); ++i)
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
            //LOG("iSegment (Etapa 1)");
            Segment *segment = (Segment *)line;
            segmentTable[segment->getName()] = new SegmentDef(segment->getName(), programCounter,0);
            currentSegment = (SegmentDef *) segmentTable.find(segment->getName())->second;

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
            //LOG("iAdd (Etapa 1)");
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

        // interrupção
        case Instruction::iINT:
            tableIntInstruction<Int>((Int *)line);
            break;

        // movimentação
        case Instruction::iMOV: {
            
            Mov * mov = (Mov *) line;
            std::set<std::string> * set = mov->getSymbolSet();
            if(set != nullptr) {
                for (std::set<std::string>::iterator it = set->begin(); it != set->end(); it++)
                {
                    if (currentSegment->getSymbol(*it) == nullptr)
                        currentSegment->setSymbol(new Symbol(*it, std::string("??")));
                }

                if(mov->isIndexed()) {
                    programCounter += 4;
                    segmentCounter += 4;
                }
                else {
                    programCounter += 3;
                    segmentCounter += 3;
                }
            }
            else {
                programCounter += 2;
                segmentCounter += 2;
            }

            //
            break;
        }

        case Instruction::iASSUME:
        {
            //LOG("iAssume (Etapa 1)");
            Assume *assume = (Assume *)line;

            if (assume->getSegmentRegister() == "cs" &&
                segmentTable.find(assume->getName()) != segmentTable.end())
            {
                assumedProgramSegment = (SegmentDef *) segmentTable.find(assume->getName())->second;
            }
            else if (assume->getSegmentRegister() == "ds" &&
                     segmentTable.find(assume->getName()) != segmentTable.end())
            {
                assumedDataSegment = (SegmentDef *) segmentTable.find(assume->getName())->second;
            }
            else if (assume->getSegmentRegister() == "ss" &&
                     segmentTable.find(assume->getName()) != segmentTable.end())
            {
                assumedStackSegment = (SegmentDef *) segmentTable.find(assume->getName())->second;
            }
            else
            {
                //LOG("iAssume ERRO");
            }
            break;
        }

        case Instruction::iORG:

            if(evaluate(((Org *) line)->getExpression(), &value) != nullptr) {
                programCounter += value;
                segmentCounter += value;
                break;
            }

            return ERRO;

        case Instruction::iDW:
        {
            
            Dw *dw = (Dw *)line;
            if (currentSegment->getSymbol(dw->getName()) == nullptr)
            {
                currentSegment->setSymbol(new Symbol(dw->getName(), std::string("??"), true, false));
            }

            if(!dw->isValueUndefined()) 
                tableVarInstruction<Dw>(dw, false);

            currentSegment->getSymbol(dw->getName())->value = std::to_string(segmentCounter/2);

            segmentCounter += 2;
            programCounter += 2;

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
            break;
        }

        case Instruction::iLABEL:
        {
            //LOG("iLabel (Etapa 1)"); 
            Label * label = (Label *) line;
            if (currentSegment->getSymbol(label->getName()) == nullptr)
            {
                currentSegment->setSymbol(new Symbol(label->getName(), std::string("??")));
            }
            //LOG(label->getName() + std::string(":") + std::to_string(segmentCounter))
            currentSegment->getSymbol(label->getName())->value = std::to_string(segmentCounter);
            currentSegment->getSymbol(label->getName())->isLabel = true;
            break;
        }

        case Instruction::iEND:
        {
            End * end = (End *) line;
            //LOG(std::string("iEND (Etapa 1)") + end->getName())
                if (currentSegment->getSymbol(end->getName()) != nullptr)
                {
                    //LOG("Passou");
                    isEnd = true;
                }
                else {
                    //LOG("ERROR");
                return ERRO;
                }
            break;
        }

        case Instruction::iENDS:
        {
            //LOG("iENDS (Etapa 1)");
            End * end = (End *) line;
            currentSegment->setSize(segmentCounter);
        } 
            break;

        case Instruction::iDIV:
        case Instruction::iRET:
        case Instruction::iNOT:
        case Instruction::iMUL:
        case Instruction::iHALT:
        case Instruction::iPUSH:
        case Instruction::iPUSHF:
        case Instruction::iPOP:
        case Instruction::iPOPF:
        default:
            //Nenhuma instrução com símbolo
            break;
        }
    }
    
    // cte1 EQU cte2+5
    // cte2 EQU cte3*3
    // cte3 EQU 4
    PendingResolution * dep;
    bool symbolWasResolved = false;
    do
    {
        for (auto i = dependencyMap.begin(); i != dependencyMap.end(); i++)
        {    
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

    return SUCCESS;
}

/*
    lida com o ExpressionEvaluator e retorna o vetor de bytes do valor resultante
*/
std::vector<byte> *Assembler::evaluate(Expression *expression, USint *valueHolder, bool * isConst)
{
    ExpressionEvaluator *evaluator = 
        new ExpressionEvaluator(expression, assumedProgramSegment, assumedDataSegment, &segmentTable);
    if (evaluator->couldNotSymbolBeResolved())
    {
        delete evaluator;
        return nullptr;
    }

    USint value = evaluator->getValue();
    if (valueHolder != nullptr)
        *valueHolder = value;

    if(isConst != nullptr)
        *isConst = evaluator->isConst();

    delete evaluator;

    return new std::vector<byte>({(byte)(value & 0xFF), (byte)(value >> 8)});
}

// produz o código de máquina associado ao código da instrução e operandos
template <class T>
std::vector<byte> * Assembler::generateAssembly(T *line)
{
    std::vector<byte> * expressValue, * opcode;

    // não depende de expressão
    if (line->getExpression() == nullptr)
    {
        segmentCounter += 2;
        programCounter += 2;
        return line->getOpCode();
    }


    segmentCounter += 3;
    programCounter += 3;

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

template<class T>
std::vector<byte> * Assembler::generateAssemblyJumps(T *line) {
    std::vector<byte> * opcode = line->getOpCode();
    
    LOG(std::string("generateAssemblyJumps:  ") + line->getLabel())
    if(assumedProgramSegment->getSymbol(line->getLabel()) == nullptr) {
        return nullptr;
    }

    // 3 - 6 = -3
    USint value = (USint) std::stoi(assumedProgramSegment->getSymbol(line->getLabel())->value);
    value -= segmentCounter;
    
    segmentCounter += 3;
    programCounter += 3;
    
    opcode->emplace_back(value & 0xFF);
    opcode->emplace_back(value >> 8);
    
    return opcode;
}

void Assembler::GetSpecialOpcode(Semantic* line)
{
    // assemblyCode.push_back(((Div *)line)->getOpcode());
    segmentCounter += 2;
    programCounter += 2;
    std::cout << "----" << std::endl;
    std::vector<byte>* lineCode = line->getOpCode();
    if (line->getType() == Instruction::iMUL){
        std::cout << "iMUL" << std::endl;
    }
    if(line->getType() == Instruction::iDIV) {
        std::cout << "iDIV" << std::endl;
    }
    for (int i = 0; lineCode != nullptr && i < lineCode->size(); i++)
    {
        std::cout << std::hex << (int)lineCode->at(i) << std::endl;
        assemblyCode.push_back(lineCode->at(i));
    }
    std::cout << "----" << std::endl;
}

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
        //LOG(std::string("Loop: " + i));
        line = lines->at(i);
        instruction = line->getType();
        showLog(instruction, std::string(""));
        if (!inSegment && instruction == Instruction::iSEGMENT)
        {
            Segment *segment = (Segment *)line;
            
            segmentCounter = 0;
            
            currentSegment = (SegmentDef *)segmentTable.find(segment->getName())->second;
            
            inSegment = true;
            
            continue;
        }

        switch (instruction)
        {

        case Instruction::iASSUME:
        {
            LOG("iAssume: etapa 2");
            Assume *assume = (Assume *)line;

            if (assume->getSegmentRegister() == "cs" &&
                segmentTable.find(assume->getName()) != segmentTable.end())
            {
                assumedProgramSegment = (SegmentDef *)segmentTable.find(assume->getName())->second;
            }
            else if (assume->getSegmentRegister() == "ds" &&
                     segmentTable.find(assume->getName()) != segmentTable.end())
            {
                assumedDataSegment = (SegmentDef *)segmentTable.find(assume->getName())->second;
            }
            else if (assume->getSegmentRegister() == "ss" &&
                     segmentTable.find(assume->getName()) != segmentTable.end())
            {
                assumedStackSegment = (SegmentDef *)segmentTable.find(assume->getName())->second;
            }
            break;
        }
        case Instruction::iEQU:
            // Nada
            break;

        case Instruction::iORG:
            lineCode = evaluate(((Org *)line)->getExpression(), &value);
            if (lineCode == nullptr)
            {
                LOG("iOrg: etapa 2");
                return ERRO;
            }
            else
            {
                programCounter += value;
                segmentCounter += value;
            }

            break;

        case Instruction::iEND:
            endLabel = ((End *)line)->getName();
            LOG(std::string("iEND: etapa 2: endLabel") + endLabel)
            label = currentSegment->getSymbol(endLabel);
            
            if (label == nullptr)
            {
                LOG("iEND: etapa 2: ERROR")
                return ERRO;
            }
        
            std::cout << "iEND:" << label->value << std::endl;
            startProgram = std::stoi(currentSegment->value) + std::stoi(label->value);
            assemblyCode.push_back(0xEE);
        
            std::cout << "deu bom" << std::endl;
            LOG("deu bom etapa 2")
            return SUCCESS;
            break;

        case Instruction::iENDS:
            endLabel = ((EndS *)line)->getName(); 

            if (endLabel != currentSegment->name)
            {
                LOG("iENDS : etapa 2")
                return ERRO;
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
        case Instruction::iPUSH:
        case Instruction::iPUSHF:
        case Instruction::iPOP:
        case Instruction::iPOPF:
            LOG("casos especiais etapa 2");
            GetSpecialOpcode(line);
            break;

        case Instruction::iMOV:
        {
            LOG("iMOV : etapa 2")

            lineCode = line->getOpCode();
            Mov * mov = (Mov *) line;
            bool isConst;

            /*
                MOV <registrador>, <registrador>
                MOV <indice>, <registrador>
                MOV <registrador>, <indice>
            */
            if (mov->getSymbolSet() == nullptr)
            {
                for (int i = 0; lineCode != nullptr && i < lineCode->size(); i++)
                {
                    assemblyCode.push_back(lineCode->at(i));
                }
                segmentCounter += 2;
                programCounter += 2;
            }
            else                                    //Mov possui expressões
            {
                // MOV <registrador>, <expressão><indice>
                if (mov->getExpression1() == nullptr && mov->isIndexed())
                {

                    for (int i = 0; lineCode != nullptr && i < lineCode->size(); i++)
                    {
                         assemblyCode.push_back(lineCode->at(i));
                    }

                    expressionValue = evaluate(mov->getExpression2(), nullptr);
                    for (int i = 0; expressionValue != nullptr && i < expressionValue->size(); i++)
                    {
                        assemblyCode.push_back(expressionValue->at(i));
                    }

                    segmentCounter += 4;
                    programCounter += 4;
                }
                // MOV <registrador>, <expressão>
                else if (mov->getExpression1() == nullptr)
                {

                    expressionValue = evaluate(mov->getExpression2(), nullptr, &isConst);

                    assemblyCode.push_back(isConst ? 0xB8 : 0xA1);
                    assemblyCode.push_back(expressionValue->at(0));
                    assemblyCode.push_back(expressionValue->at(1));

                    segmentCounter += 3;
                    programCounter += 3;
                }
                // MOV <expressão><indice>, <registrador>
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

                    segmentCounter += 4;
                    programCounter += 4;
                }
                // MOV <expressão>, <registrador>
                else if (mov->getExpression2() == nullptr)
                {

                    for (int i = 0; lineCode != nullptr && i < lineCode->size(); i++)
                    {
                        assemblyCode.push_back(lineCode->at(i));
                    }

                    expressionValue = evaluate(mov->getExpression1(), nullptr); 
                    for (int i = 0; expressionValue != nullptr && i < expressionValue->size(); i++)
                    {
                        assemblyCode.push_back(expressionValue->at(i));
                    }

                    segmentCounter += 3;
                    programCounter += 3;
                }
            }

            break;
        }
        // aritmeticas e logicas
        case Instruction::iADD:
            LOG("iADD : etapa 2");
            lineCode = generateAssembly<Add>((Add *)line);
            if (lineCode == nullptr)
            {
                return ERRO;
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
            LOG("iSUB (etapa 2)");
            lineCode = generateAssembly<Sub>((Sub *)line);
            if (lineCode == nullptr)
            {
                return ERRO;
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
            LOG("iCMP : etapa 2")
            lineCode = generateAssembly<Cmp>((Cmp *)line);
            if (lineCode == nullptr)
            {
                return ERRO;
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
            LOG("iOr: etapa 2");
            lineCode = generateAssembly<Or>((Or *)line);
            if (lineCode == nullptr)
            {
                return ERRO;
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
            LOG("And: etapa 2");
            lineCode = generateAssembly<And>((And *)line);
            if (lineCode == nullptr)
            {
                return ERRO;
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
            LOG("iXor: etapa 2");
            lineCode = generateAssembly<Xor>((Xor *)line);
            if (lineCode == nullptr)
            {
                return ERRO;
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
            lineCode = generateAssemblyJumps<Jmp>((Jmp *)line);
            if (lineCode == nullptr)
                return ERRO;
            else
                for (int i = 0; lineCode != nullptr && i < lineCode->size(); i++)
                    assemblyCode.push_back(lineCode->at(i));
            break;
            
        case Instruction::iJE:
            lineCode = generateAssemblyJumps<Je>((Je *)line);
            if (lineCode == nullptr)
                return ERRO;
            else
                for (int i = 0; lineCode != nullptr && i < lineCode->size(); i++)
                    assemblyCode.push_back(lineCode->at(i));
            break;
            
        case Instruction::iJNZ:
            lineCode = generateAssemblyJumps<Jnz>((Jnz *)line);
            if (lineCode == nullptr)
                return ERRO;
            else
                for (int i = 0; lineCode != nullptr && i < lineCode->size(); i++)
                    assemblyCode.push_back(lineCode->at(i));
            break;

        case Instruction::iJZ:
            lineCode = generateAssemblyJumps<Jz>((Jz *)line);
            if (lineCode == nullptr)
                return ERRO;
            else
                for (int i = 0; lineCode != nullptr && i < lineCode->size(); i++)
                    assemblyCode.push_back(lineCode->at(i));
            break;

        case Instruction::iJP:
            lineCode = generateAssemblyJumps<Jp>((Jp *)line);
            if (lineCode == nullptr)
                return ERRO;
            else
                for (int i = 0; lineCode != nullptr && i < lineCode->size(); i++)
                    assemblyCode.push_back(lineCode->at(i));
            break;
            
        case Instruction::iCALL:
            LOG("iCALL : etapa 2")
            lineCode = generateAssemblyJumps<Call>((Call *)line);
            if (lineCode == nullptr)
                return ERRO;
            else
                for (int i = 0; lineCode != nullptr && i < lineCode->size(); i++)
                    assemblyCode.push_back(lineCode->at(i));
            break;
            
        case Instruction::iINT:
            lineCode = generateAssembly<Int>((Int *)line);
            if (lineCode == nullptr)
                return ERRO;
            else
                for (int i = 0; lineCode != nullptr && i < lineCode->size(); i++)
                    assemblyCode.push_back(lineCode->at(i));
            break;

        case Instruction::iHALT:
            assemblyCode.push_back(line->getOpCode()->at(0));
            break;

        case Instruction::iDW:
        {
            Dw * dw = (Dw *) line;

            std::vector<byte> notInit = std::vector<byte>(2,0);

            std::vector<byte> *defaultDUP = nullptr;
            USint counterDUP = 0;

            // <identificador> DW <expressão>
            // DW <expressão>
            // <identificador> DW <expressão> DUP (<expressão>)
            // DW <expressão> DUP (<expressão>)

            if (dw->isValueUndefined())
                expressionValue = &notInit;
            else
                expressionValue = evaluate(dw->getDefaultValue(), nullptr);

            if(dw->isArray())       //possui DUP
            {
                evaluate(dw->getLength(), &counterDUP);
                
                for (int i = 0; i < counterDUP; i++)
                    for (int j = 0; defaultDUP != nullptr && j < defaultDUP->size(); j++)
                        assemblyCode.push_back(defaultDUP->at(j));

                programCounter += 2 * counterDUP;
                segmentCounter += 2 * counterDUP;
            }
            else                    //nao possui DUP
            {
                for (int i = 0; expressionValue != nullptr && i < expressionValue->size(); i++)
                {
                    assemblyCode.push_back(expressionValue->at(i));
                }
                programCounter += 2;
                segmentCounter += 2;
            }
        }
        break;

        case Instruction::iLABEL:
            // Não precisa
        break;

        default:
            //
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

            LOG(std::string("assemble: code size: ") + std::to_string(assemblyCode.size()))

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

    return preproccess(rm->analyzeText(input), k);
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
