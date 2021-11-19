#ifndef DECODER_H
#define DECODER_H

#include <unordered_map>
#include <vector>
#include <iostream>
#include <string>
#include "MacroDef.h"
#include "Symbol.h"
#include "ExpressionEvaluator.h"
#include "SegmentDef.h"
#include "../Utils.h"
#include "../Enums.h"

class Decoder
{
private:
    void mainDecoderAlgorithm();
    std::vector<Semantic *> * analyzeFile(std::string pathname);
    template <class T> void decoderForFixedOpCode(std::vector<Semantic> * SemanticVector, 
                                                  InstructionObject type, std::vector<Semantic *> * file)
    template <class T> void decoderForJump(std::vector<Semantic> * SemanticVector, 
                                           InstructionObject type, std::vector<Semantic *> * file)
    template <class T> void decoderForMoves(std::vector<Semantic> * SemanticVector, 
                                            InstructionObject type, std::vector<Semantic *> * file)
    template <class T> void decoderForStack(std::vector<Semantic> * SemanticVector, 
                                            InstructionObject type, std::vector<Semantic *> * file)
    template <class T> void decoderForUnique(std::vector<Semantic> * SemanticVector,
                                             InstructionObject type, std::vector<Semantic *> * file)
    template <class T> void decoderForUnique(std::vector<Semantic> * SemanticVector, 
                                             InstructionObject type, std::vector<Semantic *> * file)
    template <class T> void decoderForDivMul(std::vector<Semantic> * SemanticVector,
                                             InstructionObject type, std::vector<Semantic *> * file)
public:
};

#endif // DECODER_H
