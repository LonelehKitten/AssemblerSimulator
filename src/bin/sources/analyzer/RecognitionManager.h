#ifndef RECOGNITIONMANAGER_H
#define RECOGNITIONMANAGER_H

#include <vector>
#include <string>
#include <cstring>
#include <fstream> 

#include "SyntaxAnalyzer.h"
#include "Semantic.h"
#include "../Utils.h"


class RecognitionManager {

    private:
    
        SyntaxAnalyzer * analyzer;

        std::vector<std::string> * split(std::string& text);

    public:

        RecognitionManager();

        //std::vector<Semantic *> * analyze(std::string text, bool strict);
        std::vector<Semantic *> * analyzeText(std::string text);
        std::vector<Semantic *> * analyzeFile(std::string pathname);
        std::vector<Semantic *> * analyze(std::vector<std::string> * rawLines);

};

#endif // RECOGNITIONMANAGER_H
