#include "Util.h"

std::vector<std::string> Util::splitString(std::string s, std::string delimiter) {
    std::vector<std::string> splitString;

    int lastPosition = 0;
    int position;
    while ((position = s.find(delimiter, lastPosition)) != -1) {
        splitString.push_back(s.substr(lastPosition, position - lastPosition));
        lastPosition = position + delimiter.length();
    }
    splitString.push_back(s.substr(lastPosition, -1));

    return splitString;
}

std::string Util::trim(const std::string &s) {
    int first = s.find_first_not_of(' ');
    if (first == -1) {
        return "";
    }
    int last = s.find_last_not_of(' ');

    return s.substr(first, last - first + 1);
}
