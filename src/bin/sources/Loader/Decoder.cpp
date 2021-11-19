#include "Decoder.h"

#include "InstructionObject.h"
#include "../analyzer/Semantic.h"
#include "../analyzer/RecognitionManager.h"
#include "../GlobalSettings.h"

#ifdef PRODUCTION_MODE
#include "../InterfaceBus.h"
#endif

#define File std::vector<Semantic *>

// intruções:

std::vector<Semantic *> *Decoder::analyzeFile(std::string pathname)
{
    std::vector<std::string> *rawLines = new std::vector<std::string>();

    std::ifstream file;
    file.open(pathname);
    std::string temp;    
    while (!file.eof())
    {
        std::getline(file, temp);
        rawLines->emplace_back(temp);
    }
    file.close();

    return analyze(rawLines);
}

template <class T> 
void Decoder::decoderForFixedOpCode(std::vector<Semantic> * SemanticVector, InstructionObject type, std::vector<Semantic *> * file){
    SemanticVector->push_back(new T());
    //opcode fixo = add, sub, and, cmp, or, xor
    // qual o registrador (AX ou DX)
}

template <class T> 
void Decoder::decoderForJump(std::vector<Semantic> * SemanticVector, InstructionObject type, std::vector<Semantic *> * file){
    SemanticVector->push_back(new T());
    //opcode fixo = JMP, JE, JNZ, JP, oCALL
    // qual o registrador (AX ou DX)
}

template <class T> 
void Decoder::decoderForMoves(std::vector<Semantic> * SemanticVector, InstructionObject type, std::vector<Semantic *> * file){
    SemanticVector->push_back(new T());
    //opcode fixo = mov, 
    // qual o registrador (AX ou DX)
}

template <class T> 
void Decoder::decoderForStack(std::vector<Semantic> * SemanticVector, InstructionObject type, std::vector<Semantic *> * file){
    SemanticVector->push_back(new T());
    //opcode fixo = PILHAS, ret, not
    // qual o registrador (AX ou DX)
    //ret e not
}

template <class T> 
void Decoder::decoderForUnique(std::vector<Semantic> * SemanticVector, InstructionObject type, std::vector<Semantic *> * file){
    SemanticVector->push_back(new T());
    //opcode fixo = PILHAS, ret, not
    // qual o registrador (AX ou DX)
    //ret e not
}

template <class T> 
void Decoder::decoderForDivMul(std::vector<Semantic> * SemanticVector, InstructionObject type, std::vector<Semantic *> * file){
    SemanticVector->push_back(new T());
    //opcode fixo = div e mul
    // qual o registrador (AX ou DX)
}

void Decoder::mainDecoderAlgorithm()
{
    //pega semantic e converte para byte vetor
    //pega vetor de bytes e converte para semintic para semantic vector

    Semantic *line;  
    std::vector<Semantic> * SemanticVector = new std::vector<Semantic>;

    for (int i = 0; i < lines->size(); ++i)
    {
        byte b = file.nextByte();
        InstructionObject instruction = (InstructionObject) b;

        switch(instruction)
        {
            case InstructionObject::oADDOPD:
            break;
            
            case InstructionObject::oADDAX:
                DecoderForFixedOpCode(SemanticVector, InstructionObject::oADDAX, lines);
            break;
            
            case InstructionObject::oADDDX:
                DecoderForFixedOpCode(SemanticVector, InstructionObject::oADDDX, lines);
            break;

            case InstructionObject::oSUBOPD:
            break;

            case InstructionObject::oSUBAX:
                DecoderForFixedOpCode(SemanticVector, InstructionObject::oSUBAX, lines);
            break;
            
            case InstructionObject::oSUBDX:
                DecoderForFixedOpCode(SemanticVector, InstructionObject::oSUBDX, lines);
            break;

            case InstructionObject::oDIVAX:
                decoderForDivMul(SemanticVector, InstructionObject::oDIVAX, lines);
            break;
            
            case InstructionObject::oDIVSI: 
                decoderForDivMul(SemanticVector, InstructionObject::oDIVSI, lines);
            break;

            case InstructionObject::oMULAX:
                decoderForDivMul(SemanticVector, InstructionObject::oMULAX, lines);
            break;
            
            case InstructionObject::oMULSI:
                decoderForDivMul(SemanticVector, InstructionObject::oMULSI, lines);
            break;
            
            case InstructionObject::oCMPDX:
                DecoderForFixedOpCode(SemanticVector, InstructionObject::oCMPDX, lines);
            break;

            case InstructionObject::oCMPOPD:
            break;

            case InstructionObject::oNOTAX:
                decoderForStack(SemanticVector, InstructionObject::oNOTAX, lines);
            break; 

            case InstructionObject::oANDOPD:
            break;
            
            case InstructionObject::oANDAX:
                DecoderForFixedOpCode(SemanticVector, InstructionObject::oANDAX, lines);
            break;

            case InstructionObject::oANDDX:
                DecoderForFixedOpCode(SemanticVector, InstructionObject::oANDDX, lines);            
            break;

            case InstructionObject::oOROPD:
            break;
            
            case InstructionObject::oORAX:
                DecoderForFixedOpCode(SemanticVector, InstructionObject::oORAX, lines);
            break;
                    
            case InstructionObject::oORDX:
                DecoderForFixedOpCode(SemanticVector, InstructionObject::oORDX, lines);
            break;
            
            case InstructionObject::oXOROPD:
            break;
            
            case InstructionObject::oXORAX:
                DecoderForFixedOpCode(SemanticVector, InstructionObject::oXORAX, lines);
            break;
            
            case InstructionObject::oXORDX:
                DecoderForFixedOpCode(SemanticVector, InstructionObject::oXORDX, lines);
            break;

            case InstructionObject::oJMP:  
                DecoderForJump(SemanticVector, InstructionObject::oJMP, lines);    
            break;
            
            case InstructionObject::oJE:
                decoderForJump(SemanticVector, InstructionObject::oJE, lines);
            break;
            
            case InstructionObject::oJNZ:
                decoderForJump(SemanticVector, InstructionObject::oJNZ, lines);
            break; 
            
            case InstructionObject::oJP:
                DecoderForJump(SemanticVector, InstructionObject::oJP, lines);    
            break;
            
            case InstructionObject::oCALL:
                decoderForJump(SemanticVector, InstructionObject::oCALL, lines);
            break;

            case InstructionObject::oINT:
                //fazer esse aq separado
            break;

            case InstructionObject::oRET:
                decoderForStack(SemanticVector, InstructionObject::oRET, lines);
            break;

            case InstructionObject::oPOP:
                decoderForStack(SemanticVector, InstructionObject::oPOP, lines);
            break;

            case InstructionObject::oPUSH:
                decoderForStack(SemanticVector, InstructionObject::oPUSH, lines);
            break;

            case InstructionObject::oPOPF:
                decoderForStack(SemanticVector, InstructionObject::oPOPF, lines);
            break;

            case InstructionObject::oPUSHF:
                decoderForStack(SemanticVector, InstructionObject::oPUSHF, lines);
            break;

            case InstructionObject::oMOVREGIND:
                decoderForMoves(SemanticVector, InstructionObject::oMOVREGIND, lines);
            break;
            
            case InstructionObject::oMOVINDREG:
                decoderForMoves(SemanticVector, InstructionObject::oMOVINDREG, lines);
            break;
            
            case InstructionObject::oMOVREGREG:
                decoderForMoves(SemanticVector, InstructionObject::oMOVREGREG, lines);
            break;
            
            case InstructionObject::oMOVOPDINDREG: 
                decoderForMoves(SemanticVector, InstructionObject::oMOVOPDINDREG, lines);
            break;

            case InstructionObject::oMOVOPDREG:
                decoderForMoves(SemanticVector, InstructionObject::oMOVOPDREG, lines);
            break;
            
            case InstructionObject::oMOVREGOPDIND: 
                decoderForMoves(SemanticVector, InstructionObject::oMOVREGOPDIND, lines);
            break;
            
            case InstructionObject::oMOVREGOPD:
                decoderForMoves(SemanticVector, InstructionObject::oMOVREGOPD, lines);
            break;

            case InstructionObject::oASSUME:
                decoderForUnique(SemanticVector, InstructionObject::oASSUME, lines);
            break;
            
            case InstructionObject::oORG:
                decoderForUnique(SemanticVector, InstructionObject::oORG, lines);
            break;

            case InstructionObject::oDW:
                decoderForUnique(SemanticVector, InstructionObject::oDW, lines);
            break;

            case InstructionObject::oEQU:
                decoderForUnique(SemanticVector, InstructionObject::oEQU, lines);
            break;

            case InstructionObject::oLABEL: 
                decoderForUnique(SemanticVector, InstructionObject::oLABEL, lines);
            break;

            case InstructionObject::oENDS:
                decoderForUnique(SemanticVector, InstructionObject::oENDS, lines);
            break;

            case InstructionObject::oINVALID:
                decoderForUnique(SemanticVector, InstructionObject::oINVALID, lines);
            break;

            case InstructionObject::oHALT:
                decoderForUnique(SemanticVector, InstructionObject::oHALT, lines);
            break;
        }

    }

}