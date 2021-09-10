#include "SyntaxAnalyzer.h"

SyntaxAnalyzer::SyntaxAnalyzer() {
    this->scanner = new LexiconScanner();
}

void SyntaxAnalyzer::set(std::string line, bool last) {
    this->line = line;
    this->last = last;
}

void SyntaxAnalyzer::predict() {
    log("predict");

    char c;
    for (int i = 0; i < (int) this->line.length(); i++) {
        c = this->line[i];
        if(c == ' ') continue;
        this->predict(c);
        break;
    }
}

void SyntaxAnalyzer::predict(char c) {



}

bool SyntaxAnalyzer::init() {
    log("init");
    this->row.clear();
    this->error = false;
    this->scanner->setLine(this->line);
    while (true) {
        if(this->state(this) && !this->error) {
            return true;
        }
        if(this->error) break;
    }
    return false;
}

bool SyntaxAnalyzer::check() {

}

void SyntaxAnalyzer::getRow() {

}

void SyntaxAnalyzer::log(std::string msg){
    std::cout << "------------------------------" << std::endl;
    std::cout << "%c DEBUG: " << msg << std::endl;
    std::cout << "------------------------------" << std::endl;
}
