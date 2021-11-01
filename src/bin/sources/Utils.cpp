#include "Utils.h"

namespace Utils {
    bool isSpecialCharactere(char c){
        if(
                (c >= 'a' && c <= 'z') || 
                (c >= 'A' && c <= 'Z') || 
                (c >= '0' && c <= '9') || 
                c == '_' || c == ' ' || c == '\n' || c == '\0'
            ){
                return false;
            }
            return true;
    }

    bool isSpecialCharactere(std::string str){
        char c = str.c_str()[0];
        if(
                (c >= 'a' && c <= 'z') || 
                (c >= 'A' && c <= 'Z') || 
                (c >= '0' && c <= '9') || 
                c == '_' || c == ' ' || c == '\n' || c == '\0'
            ){
                return false;
            }
            return true;
    }

    std::vector<std::string> * split(std::string& text, char divider) {

        std::vector<std::string> * lines = new std::vector<std::string>();

        int begin = 0, end = 1, it = 0;
        while(it < (int) text.size()) {

            if(it == (int) text.size()-1) {
                lines->emplace_back(text.substr(begin, end));
                break;
            }

            if(text[it] == divider) {
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

    std::string replaceAll(std::string str, const std::string &from, const std::string &to) {

        if (from.empty())
            return;
        size_t start_pos = 0;
        while ((start_pos = str.find(from, start_pos)) != std::string::npos)
        {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }

        return str;

    }

    std::string toLowerCase(std::string str) {
        std::transform(
            str.begin(), 
            str.end(), 
            str.begin(), 
            [](unsigned char c) {
                return std::tolower(c);
            });

        return str;
    }


};