#ifndef DICTIONARIES_H
#define DICTIONARIES_H

enum class AutomatonPattern {
    pLABEL = 0,
    pSYMBOL
};

enum class TokenTypes {
    tNULL_TYPE = -1,
    tBLOCKDEF,
    tBLOCKEND,
    tEND,
    tOPERATION,
    tREGISTER,
    tASSUME,
    tExpARITHMETICb,
    tExpARITHMETICu,
    tExpLOGICALb,
    tExpLOGICALu,
    tExpRELATIONALb,
    tPC,
    tOFFSET,
    tINDEX_OP,
    tINDEX_ED,
    tCOLON,
    tExpPRECEDENCE_OP,
    tExpPRECEDENCE_ED,
    tVARDEF,
    tVECFILL,
    tCONSTDEF,
    tSEPARATOR,
    tUNDEFINED,
    tSTACK,
    tORG,
    tIDENTIFIER
};

enum class TokenNames {
    nNULL_TYPE = -1,
    nDirMACRO,
    nDirPROC,
    nDirSEGMENT,
    nDirENDM,
    nDirENDP,
    nDirENDS,
    nDirEND,
    // OPERAÇÕES
    nOpADD,
    nOpSUB,
    nOpMUL,
    nOpDIV,
    nOpOR,
    nOpAND,
    nOpXOR,
    nOpNOT,
    nOpJMP,
    nOpJE,
    nOpJNZ,
    nOpJZ,
    nOpJP,
    nOpCALL,
    nOpINT,
    nOpRET,
    nOpMOV,
    nOpPOP,
    nOpPUSH,
    nOpPOPF,
    nOpPUSHF,
    // REGISTRADORES
    nRegAX,
    nRegDX,
    nRegSI,
    nRegSP,
    nRegCS,
    nRegDS,
    nRegSS,
    // OPERAÇÕES DE EXPRESSÃO (MONTADOR)
    nExpADD,
    nExpSUB,
    nExpMUL,
    nExpDIV,
    nExpMOD,
    nExpOR,
    nExpAND,
    nExpXOR,
    nExpNOT,
    nExpEQ,
    nExpNE,
    nExpLT,
    nExpLE,
    nExpGT,
    nExpGE,
    nExpPRECEDENCE_OP,
    nExpPRECEDENCE_ED,
    // DECLARATION
    nASSUME,
    nDW,
    nDUP,
    nEQU,
    nUNDEFINED,
    // OTHERS
    nINDEX_OP,
    nINDEX_ED,
    nSEPARATOR,
    nCOLON,
    nPC,
    nORG,
    nOFFSET,
    nSTACK
};


#endif // DICTIONARIES_H
