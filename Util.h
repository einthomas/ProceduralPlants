#pragma once

#define GLEW_STATIC
#include <vector>
#include <sstream>

class Util {
public:
    static std::vector<std::string> splitString(std::string s, std::string delimiter);
    static std::string trim(const std::string &s);

private:
    static const std::string delimiters;
};
