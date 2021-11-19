#pragma once

enum InstructionObject{
    //aritmeticas

    //iADD,
    oADDAX,
    oADDDX,
    oADDOPD,

    //iSUB,
    oSUBAX,
    oSUBDX,
    oSUBOPD,

    //iDIV,
    oDIVSI,
    oDIVAX,
    
    //iMUL,
    oMULSI,
    oMULAX,
    
    //iCMP
    oCMPDX,
    oCMPOPD,
    
    //logicas
    
    //iOR,
    oORAX,
    oORDX,
    oOROPD,

    //iAND,
    oANDAX,
    oANDDX,
    oANDOPD,
    
    //iXOR,
    oXORAX,
    oXORDX,
    oXOROPD,


    //iNOT
    oNOTAX,
    
    //desvio
    oJMP,
    oJE,
    oJNZ,
    oJZ,
    oJP,
    oCALL,
    oINT,
    oRET,
    
    //movimentação
    oMOVREGREG, // mov <registrador>, <registrador>
    oMOVREGOPD, // mov <registrador>, <expression>
    oMOVREGIND, // mov <registrador>, [SI]
    oMOVREGOPDIND, // mov <registrador>, <expression>[SI]
    oMOVOPDREG, // mov <expression>, <registrador>
    oMOVINDREG, // mov [SI], <registrador>
    oMOVOPDINDREG, // mov <expression>[SI], <registrador>
    
    //pilha
    oPOP,
    oPUSH,
    oPOPF,
    oPUSHF,

    //instruções ao processo de montagem
    oEND,
    oSEGMENT,
    oENDS,
    oDW,
    //iDUP, n precisa
    oEQU,
    ORG,
    //iOFFSET, n precisa
    oASSUME,
    oPROC,
    oENDP,
    oMACRO,
    oENDM,
    oLABEL,
    oHALT

    //se mudar esse enum tem que mudar o do ObjectFileGenerator.cpp também

    //iMACROCONTENT - n precisa
    //iMACROCALL - n precisa
};
