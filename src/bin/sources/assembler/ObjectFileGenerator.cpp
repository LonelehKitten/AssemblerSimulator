#include "ObjectFileGenerator.h"

#include "../analyzer/Semantic.h"
#include "../analyzer/RecognitionManager.h"
#include "../GlobalSettings.h"

#ifdef PRODUCTION_MODE
#include "../InterfaceBus.h"
#endif

#define File std::vector<Semantic *>

// intruções:
enum InstructionObject{
    //aritmeticas

    //iADD,
    iADDAX,
    iADDDX,
    iADDOPD,

    //iSUB,
    iSUBAX,
    iSUBDX,
    iSUBOPD,

    //iDIV,
    iDIVSI,
    iDIVAX,
    
    //iMUL,
    iMULSI,
    IMULAX,
    
    //iCMP
    iCMPDX,
    iCMPOPD,
    
    //logicas
    
    //iOR,
    iORAX,
    iORDX,
    iOROPD,

    //iAND,
    iANDAX,
    iANDDX,
    iANDOPD,
    
    //iXOR,
    iXORAX,
    iXORDX,
    iXOROPD,


    //iNOT
    iNOTAX,
    
    //desvio
    iJMP,
    iJE,
    iJNZ,
    iJZ,
    iJP,
    iCALL,
    iINT,
    iRET,
    
    //movimentação
    iMOVREGREG, // mov <registrador>, <registrador>
    iMOVREGOPD, // mov <registrador>, <expression>
    iMOVREGIND, // mov <registrador>, [SI]
    iMOVREGOPDIND, // mov <registrador>, <expression>[SI]
    iMOVOPDREG, // mov <expression>, <registrador>
    iMOVINDREG, // mov [SI], <registrador>
    iMOVOPDINDREG, // mov <expression>[SI], <registrador>
    
    //pilha
    iPOP,
    iPUSH,
    iPOPF,
    iPUSHF,

    //instruções ao processo de montagem
    iEND,
    iSEGMENT,
    iENDS,
    iDW,
    //iDUP, n precisa
    iEQU,
    iORG,
    //iOFFSET, n precisa
    iASSUME,
    iPROC,
    iENDP,
    iMACRO,
    iENDM,
    iLABEL,
    iINVALID,
    iHALT

    //se mudar esse enum tem que mudar o do loader.cpp também

    //iMACROCONTENT - n precisa
    //iMACROCALL - n precisa
};

ObjectFileGenerator::ObjectFileGenerator(std::vector<Semantic *> *lines) : lines(lines)
{   
}


void ObjectFileGenerator::Generate()
{
    Semantic *line;
    std::vector<byte> * file = new std::vector<byte>;

    for (int i = 0; i < lines->size(); ++i)
    {
        line = lines->at(i);
        instruction = line->getType();
        
        switch(instruction)
        {
            case Instruction::iADD:
                if( ((Add *) line)->getExpression() != nullptr ){
                    //tem expressão
                    file->push_back(InstructionObject::iADDOPD);
                    exportExpression(line->getExpression(), file);
                }else{
                    //não tem expressão
                    if(((Add *) line).getRegister() == "ax"){ // "", "ax", "dx"
                        file->push_back(InstructionObject::iADDAX);
                        break;
                    }else{
                        file->push_back(InstructionObject::iADDDX);
                    }   
                }
            break;

            case Instruction::iSUB:
                if( ((Sub *) line)->getExpression() != nullptr ){
                    file->push_back(InstructionObject::iSUBOPD);
                    exportExpression(line->getExpression(), file);
                }else{
                    if( ((Sub *)line).getRegister() = "ax" ){
                        file->push_back(InstructionObject::iSUBAX);
                        break;
                    }else{
                        file->push_back(InstructionObject::iSUBDX)
                    }
                }
            break;
            
            case Instruction::iDIV:
                if(((Div *) line).getRegister() == "ax"){
                    file->push_back(InstructionObject::iDIVAX);
                    break;
                }else{
                    file->push_back(InstructionObject::iDIVSI);
                }   
            break;

            case Instruction::iMUL:
             if(((Mul *) line).getRegister() == "ax"){
                    file->push_back(InstructionObject::iMULAX);
                    break;
                }else{
                    file->push_back(InstructionObject::iMULSI);
                } 
            break;
            
            case Instruction::iCMP:
            if(((Cmp *) line).getRegister() == "dx"){
                    file->push_back(InstructionObject::iCMPDX);
                    break;
                }else{
                    file->push_back(InstructionObject::iCMPOPD);
            } 
            break;
            
            case Instruction::iNOT:
                file->push_back(InstructionObject::iNOTAX);
            break;     

            case Instruction::iAND:
                if( ((And *) line)->getExpression() != nullptr ){
                    file->push_back(InstructionObject::iANDOPD);
                    exportExpression(line->getExpression(), file);
                }else{
                    if( ((And *)line).getRegister() = "ax" ){
                        file->push_back(InstructionObject::iANDAX);
                        break;
                    }else{
                        file->push_back(InstructionObject::iANDDX)
                    }
                }
            break;

            case Instruction::iOR:
                if( ((Or *) line)->getExpression() != nullptr ){
                    file->push_back(InstructionObject::iOROPD);
                    exportExpression(line->getExpression(), file);
                }else{
                    if( ((Or *)line).getRegister() = "ax" ){
                        file->push_back(InstructionObject::iORAX);
                        break;
                    }else{
                        file->push_back(InstructionObject::iORDX)
                    }
                }
            break;
            
            case Instruction::iXOR:
                if( ((Xor *) line)->getExpression() != nullptr ){
                    file->push_back(InstructionObject::iXOROPD);
                    exportExpression(line->getExpression(), file);
                }else{
                    if( ((Xor *)line).getRegister() = "ax" ){
                        file->push_back(InstructionObject::iXORAX);
                        break;
                    }else{
                        file->push_back(InstructionObject::iXORDX)
                    }
                }
            break;

//-----------PARA ARRUMAR--------------------------------
            case Instruction::iJMP:
                //JumpsIterator(InstructionObject type, std::vector<byte> * file, Semantic * line)
                JumpsIterator(InstructionObject::iJMP, file, line);        
            break;
            
            case Instruction::iJE:
                JumpsIterator(InstructionObject::iJE, file, line);
            break;
            
            case Instruction::iJNZ:
                JumpsIterator(InstructionObject::iJNZ, file, line);
            break; 
            
            case Instruction::iJP:
                JumpsIterator(InstructionObject::iJP, file, line);
            break;
            
            case Instruction::iCALL:
                JumpsIterator(InstructionObject::iCALL, file, line);
            break;

            case Instruction::iINT:
                file->push_back(InstructionObject::iINT);
                
            break;

            case Instruction::iRET:
                file->push_back(InstructionObject::iRET); 
            break;
//----------------------------------------------------------


            case Instruction::iPOP:
                file->push_back(InstructionObject::iPOP);
            break;

            case Instruction::iPUSH:
                file->push_back(InstructionObject::iPUSH);
            break;

            case Instruction::iPOPF:
                file->push_back(InstructionObject::iPOPF);
            break;

            case Instruction::iPUSHF:
                file->push_back(InstructionObject::iPUSHF);
            break;

            case Instruction::iMOV: {
                Mov * mov = (Mov *) line;
                if(mov->getExpression1() == nullptr && mov->getExpression2() == nullptr){
                    
                    if(mov->isIndexed() && mov->getOperand1() != "") {
                        file->push_back(InstructionObject::iMOVREGIND);
                        exportString(mov->getOperand1(), file);
                    }
                    if(mov->isIndexed() && mov->getOperand2() != "") {
                        file->push_back(InstructionObject::iMOVINDREG);
                        exportString(mov->getOperand2(), file);
                    }
                    else {
                        file->push_back(InstructionObject::iMOVREGREG);
                        exportString(mov->getOperand1(), file);
                        exportString(mov->getOperand2(), file);
                    }
                }
                else if(mov->getExpression1() != nullptr) { // expressão no operando 1
                    file->push_back((byte) (mov->isIndexed() ? InstructionObject::iMOVOPDINDREG : InstructionObject::iMOVOPDREG));
                    exportExpression(mov->getExpression1(), file);
                }
                else if(mov->getExpression2() != nullptr) { // expressão no operando 2
                    file->push_back((byte) (mov->isIndexed() ? InstructionObject::iMOVREGOPDIND : InstructionObject::iMOVREGOPD));
                    exportExpression(mov->getExpression2(), file);
                }
            }
            break;


//-----------PARA ARRUMAR--------------------------------
            case Instruction::iASSUME:
                file->push_back(InstructionObject::iASSUME);
            break;
            
            case Instruction::iORG:
                file->push_back(InstructionObject::iORG);
            break;

            case Instruction::iDW:
                file->push_back(InstructionObject::iDW);
            break;

            case Instruction::iEQU:
                file->push_back(InstructionObject::iEQU);
            break;

            case Instruction::iLABEL: // loop1: 
                file->push_back(InstructionObject::iLABEL);
                exportString(((Label *)line)->getName(), file)
            break;

            case Instruction::iENDS:
                file->push_back(InstructionObject::iENDS);
            break;
//----------------------------------------------------------
        }

    }

}

template<class T>
void ObjectFileGenerator::JumpsIterator(InstructionObject type, std::vector<byte> * file, Semantic * line){
    file->push_back(type);
    std::string Nome = ((T *) line)->getName();
    exportString(Nome, file);
}

void ObjectFileGenerator::exportString(std::string str, std::vector<byte> * file) {
    for(auto it_it = str.begin(); it_it != str.end(); it_it++){
        file->push_back(it_it);
    }
    file->push_back('\0');
}

void ObjectFileGenerator::exportExpression(std::vector<Token *> * expression, std::vector<byte> * file) {
    file->emplace_back((byte) expression->size());
    for(auto token : expression) {
        switch(token->getType()) {
            TokenTypes::tIDENTIFIER:
            TokenTypes::tDECIMAL:
            TokenTypes::tHEXADECIMAL:
            TokenTypes::tBINARY:
            TokenTypes::tCHARACTERE:
                file->emplace_back(0xFF);
                file->emplace_back((byte) token->getType());
                exportString(token->getToken(), file);
                break;
            default: 
                file->emplace_back((byte) token->getName());
        }
    }
}