#include "Loader.h"

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

    //se mudar esse enum tem que mudar o do ObjectFileGenerator.cpp também

    //iMACROCONTENT - n precisa
    //iMACROCALL - n precisa
};

Loader::Loader()(std::vector<Semantic *> *lines) : lines(lines)
{   
}

void Loader::Decoder()
{
    // verificar pelos bytes
    // primeiro byte é a instrução

    Semantic *line;
    std::vector<byte> * file = new std::vector<byte>;

    for (int i = 0; i < lines->size(); ++i)
    {
        line = lines->at(i);
        instruction = line->getType();
        
        switch(instruction)
        {
            case InstructionObject::iADDOPD:
            // o q fazer?
            break;
            
            case InstructionObject::iADDAX:
            break;
            
            case InstructionObject::iADDDX:
            break;

            case InstructionObject::iSUBOPD:
            break;

            case InstructionObject::iSUBAX:
            break;
            
            case InstructionObject::iSUBDX:
            break;

            case InstructionObject::iDIVAX:
            break;
            
            case InstructionObject::iDIVSI: 
            break;

            case InstructionObject::iMULAX:
            break;
            
            case InstructionObject::iMULSI:
            break;
            
            case InstructionObject::iCMPDX:
            break;

            case InstructionObject::iCMPOPD:
            break;

            case InstructionObject::iNOTAX:
            break;     

            case InstructionObject::iANDOPD:
            break;
            
            case InstructionObject::iANDAX:
            break;

            case InstructionObject::iANDDX:
            break;

            case InstructionObject::iOROPD:
            break;
            
            case InstructionObject::iORAX:
            break;
                    
            case InstructionObject::iORDX:
            break;
            
            case InstructionObject::iXOROPD:
            break;
            
            case InstructionObject::iXORAX:
            break;
            
            case InstructionObject::iXORDX:
            break;

            case InstructionObject::iJMP:      
            break;
            
            case InstructionObject::iJE:
            break;
            
            case InstructionObject::iJNZ:
            break; 
            
            case InstructionObject::iJP:
            break;
            
            case InstructionObject::iCALL:
            break;

            case InstructionObject::iINT:
            break;

            case InstructionObject::iRET:
            break;

            case InstructionObject::iPOP:
            break;

            case InstructionObject::iPUSH:
            break;

            case InstructionObject::iPOPF:
            break;

            case InstructionObject::iPUSHF:
            break;

            case InstructionObject::iMOVREGIND:
            break;
            
            case InstructionObject::iMOVINDREG:
            break;
            
            case InstructionObject::iMOVREGREG:
            break;
            
            case InstructionObject::iMOVOPDINDREG: 
            break;

            case InstructionObject::iMOVOPDREG:
            break;
            
            case InstructionObject::iMOVREGOPDIND: 
            break;
            
            case InstructionObject::iMOVREGOPD:
            break;

            case InstructionObject::iASSUME:
            break;
            
            case InstructionObject::iORG:
            break;

            case InstructionObject::iDW:
            break;

            case InstructionObject::iEQU:
            break;

            case InstructionObject::iLABEL: 
            break;

            case InstructionObject::iENDS:
            break;

            case InstructionObject::iINVALID:
            break;

            case InstructionObject::iHALT:
            break;
        }

    }

}