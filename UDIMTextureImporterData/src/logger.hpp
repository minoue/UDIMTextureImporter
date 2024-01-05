#pragma once


#include <string>

class Logger {
public:
    static void init(const std::string& filePath);
    static void write(const std::string& log);
    static void close();
 
private:
    static std::string filePath;
    static FILE* file;
};
