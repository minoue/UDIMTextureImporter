#include <filesystem>

#include "logger.hpp"
#include "timer.hpp"


std::string Logger::filePath = "";
FILE* Logger::file = NULL;


void Logger::init(const std::string& path) {
    std::filesystem::path gozPath(path);
    gozPath.replace_filename("UDIMImporter.log");
    
    file = fopen(gozPath.string().c_str(), "w");
    if (file == NULL) {
        // err
    }

    char line[128];
    snprintf(line, sizeof(line), "[%s] : UDIMImporter started.\n", Timer::getCurrentTime().c_str());
    fputs(line, file);
}

void Logger::write(const std::string& message) {
    char line[128];
    snprintf(line, sizeof(line), "[%s] : %s\n", Timer::getCurrentTime().c_str(), message.c_str());
    fputs(line, file);
}

void Logger::close() {
    fclose(file);
}
