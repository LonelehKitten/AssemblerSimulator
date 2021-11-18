#include "RecognitionManager.h"
#include "../GlobalSettings.h"
#ifdef PRODUCTION_MODE
#include "../InterfaceBus.h"
#endif

RecognitionManager::RecognitionManager()
{
    this->analyzer = new SyntaxAnalyzer();
}

std::vector<Semantic *> *RecognitionManager::analyzeFile(std::string pathname)
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

std::vector<Semantic *> *RecognitionManager::analyzeText(std::string text)
{

    std::vector<std::string> *rawLines = Utils::split(text, '\n');

    TEST(
        for (int i = 0; i < (int)rawLines->size(); i++)
                std::cout
            << "l" << i << ":  " << rawLines->at(i) << std::endl;
        std::cout << "size: " << text.size() << std::endl;)

    return analyze(rawLines);
}

std::vector<Semantic *> *RecognitionManager::analyze(std::vector<std::string> *rawLines)
{
    std::vector<Semantic *> *lines = new std::vector<Semantic *>();

    for (int i = 0; i < (int)rawLines->size(); i++)
    {
        analyzer->set(rawLines->at(i), i + 1 < (int)rawLines->size());
        if (analyzer->check())
        {
            bool status = analyzer->init();
            if (!status)
            {
                PRODUCTION(InterfaceBus::getInstance().dispatchLog(analyzer->getErrorMessage(i + 1), LogStatus::ERROR))
                TEST(std::cout << analyzer->getErrorMessage(i + 1) << std::endl)
            }

            lines->emplace_back(analyzer->getRow());
        }
    }
    delete rawLines;
    return lines;
}

// 0 1 2 3 4 5 6 begin
// 1 2 3 4 5 6 7 end
// c a r r o . c
