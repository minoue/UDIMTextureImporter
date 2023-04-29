#include <cstddef>
#include <sstream>

#include "util.hpp"

Utils::Utils() {};

Utils::~Utils() {};

size_t Utils::split(const std::string& txt, std::vector<std::string>& strs, char ch)
{
    size_t pos = txt.find(ch);
    size_t initialPos = 0;
    strs.clear();

    // Decompose statement
    while (pos != std::string::npos) {
        strs.push_back(txt.substr(initialPos, pos - initialPos));
        initialPos = pos + 1;

        pos = txt.find(ch, initialPos);
    }

    // Add the last one
    strs.push_back(txt.substr(initialPos, std::min(pos, txt.size()) - initialPos + 1));

    return strs.size();
}

std::string Utils::pathGetUdim(const std::string path)
{
    std::vector<std::string> pathSplit;
    split(path, pathSplit, '.');
    size_t n = pathSplit.size();
    std::string& word = pathSplit[n - 2];
    size_t strLen = word.length();

    std::string udim = word.substr(strLen - 4);

    return udim;
}
