#include "RecognitionManager.h"
#include "../GlobalSettings.h"

#ifdef PRODUCTION_MODE
#include "../InterfaceBus.h"
#endif

RecognitionManager::RecognitionManager() {
    this->analyzer = new SyntaxAnalyzer();
}

std::vector<Semantic *> * RecognitionManager::analyze(std::string text, bool strict) {

    std::vector<std::string> * rawLines = Utils::split(text, '\n');

    TEST(
        for(int i = 0; i < (int) rawLines->size(); i++)
            std::cout << "l" << i << ":  " << rawLines->at(i) << std::endl;
        std::cout << "size: " << text.size() << std::endl;
    )

    std::vector<Semantic *> * lines = new std::vector<Semantic *>();

    for(int i = 0; i < (int) rawLines->size(); i++) {
        analyzer->set(rawLines->at(i), i+1 < (int) rawLines->size());
        if(analyzer->check()) {
            bool status = analyzer->init();
            if(!status) {
                PRODUCTION(InterfaceBus::getInstance().dispatchLog(analyzer->getErrorMessage(i+1), LogStatus::ERROR))
                TEST(std::cout << analyzer->getErrorMessage(i+1) << std::endl)
            }
            if(!status && strict) {

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

// 0 1 2 3 4 5 6 begin
// 1 2 3 4 5 6 7 end
// c a r r o . c
