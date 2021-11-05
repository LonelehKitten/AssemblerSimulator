#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <functional>

namespace Utils {


    std::vector<std::string> * split(std::string& text, char divider);

    std::string replaceAll(std::string str, const std::string &from, const std::string &to);

    std::string toLowerCase(std::string str);

    bool isSpecialCharactere(char c);
    bool isSpecialCharactere(std::string str);
};

#endif /* UTILS_H */