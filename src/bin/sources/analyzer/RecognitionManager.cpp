#include "RecognitionManager.h"

RecognitionManager::RecognitionManager() {
    this->analyzer = new SyntaxAnalyzer();
}

std::vector<Semantic *> * RecognitionManager::analyze(std::string text, bool strict) {

    std::vector<std::string> * rawLines = split(text);

    DEBUG(
        for(int i = 0; i < (int) rawLines->size(); i++)
            std::cout << "l" << i << ":  " << rawLines->at(i) << std::endl;
        std::cout << "size: " << text.size() << std::endl;
    )

    std::vector<Semantic *> * lines = new std::vector<Semantic *>();

    for(int i = 0; i < (int) rawLines->size(); i++) {
        analyzer->set(rawLines->at(i), i < (int) rawLines->size());
        if(analyzer->check()) {
            if(!analyzer->init() && strict) {

                for(int j = 0; j < (int) lines->size(); j++)
                    lines->at(j)->~Semantic();

                rawLines->~vector<std::string>();

                return nullptr;
            }
            lines->emplace_back(analyzer->getRow());
        }
    }

    return lines;

}

std::vector<std::string> * RecognitionManager::split(std::string& text) {

    std::vector<std::string> * lines = new std::vector<std::string>();

    int begin = 0, end = 1, it = 0;
    while(it < (int) text.size()) {

        if(it == (int) text.size()-1) {
            lines->emplace_back(text.substr(begin, end));
            break;
        }

        if(text[it] == '\n') {
            lines->emplace_back(text.substr(begin, end));
            it++;
            begin = it;
            end = 1;
            continue;
        }

        it++;
        end++;
    }

    return lines;

}
// 0 1 2 3 4 5 6 begin
// 1 2 3 4 5 6 7 end
// c a r r o . c
