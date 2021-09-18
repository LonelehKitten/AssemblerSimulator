#ifndef RECOGNITIONMANAGER_H
#define RECOGNITIONMANAGER_H

#include <vector>
#include <string>
#include <cstring>

#include "SyntaxAnalyzer.h"
#include "Semantic.h"


class RecognitionManager {

    private:
        SyntaxAnalyzer * analyzer;

        std::vector<std::string> * split(std::string& text);

    public:
        RecognitionManager();

    std::vector<Semantic *> * analyze(std::string text);


};

#endif // RECOGNITIONMANAGER_H
