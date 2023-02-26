#include <cstdlib>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>

#include "displacementImporter.hpp"

#define EXPORT __attribute__((visibility("default")))


float EXPORT ImportVectorDisplacement(char* textFromZBrush,
                                      double valueFromZBrush,
                                      char* pOptBuffer1,
                                      int optBuffer1Size,
                                      char* pOptBuffer2,
                                      int optBuffer2Size,
                                      char** zData)
{

    std::string logPath;

#ifdef _WIN32
    logPath = "";
#endif

#ifdef __APPLE__
    logPath = "/tmp/displacementImporter.log";
#endif

    std::string objFile(textFromZBrush);    // from zbrush
    std::vector<std::string> texture_paths;
    std::string cmdString(pOptBuffer1);
    int cmdLength = static_cast<int>(cmdString.length());

    std::string path;

    for (int i = 0; i < cmdLength; i++) {
        const char c = pOptBuffer1[i];
        if (c == ':') {
            texture_paths.push_back(path);
            path.clear();
        } else {
            path.push_back(c);
        }
    }

    std::string cmd = "/Applications/ZBrush\\ 2022/ZStartup/ZPlugs64/displacementImporter/objModifier";
    cmd.append(" -i " + objFile);
    cmd.append(" -o ZBrush_displacementImporter_in");
    cmd.append(" -t");

    for (std::string& tex : texture_paths) {
        cmd.append(" " + tex);
    }
    cmd.append(" > " + logPath);

    auto ret = system(nullptr);

    if (ret != 0) {
        std::cout << "shell available" << std::endl;
        int r = system(cmd.c_str());
        if (r != 0) {
            // not successful
            strcpy(pOptBuffer2, "Failed to run the plugin command. Check the log and send it to developer.");
            return 1.0;
        }
    } else {
        std::cout << "shell is NOT available" << std::endl;
        return 1.0;
    }

    // log
    std::ofstream logOfs(logPath, std::ios::app);
    if (!logOfs) {
        strcpy(pOptBuffer2, "Failed to open the log file.");
        return 1.0;
    } 
    logOfs << "Command running: " << std::endl;
    logOfs << cmd << std::endl;
    logOfs.close();

    return 0.0f;
}
