#ifndef __UTIL_HPP__
#define __UTIL_HPP__

#include <string>
#include <vector>

class Utils {
public:
    Utils();
    ~Utils();
    static size_t split(const std::string& txt, std::vector<std::string>& strs, char ch);
    static std::string pathGetUdim(const std::string path);

private:
};

#endif // __UTIL_HPP__
