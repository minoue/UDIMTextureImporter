#include <cstdlib>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <filesystem>

#include "displacementImporter.hpp"


float EXPORT ImportDisplacement(char* textFromZBrush,
                                      double dspMode,
                                      char* pOptBuffer1,
                                      int optBuffer1Size,
                                      char* pOptBuffer2,
                                      int optBuffer2Size,
                                      char** zData)
{
    std::vector<std::string> cmdVector;
    std::string cmdString(pOptBuffer1);

    int cmdLength = static_cast<int>(cmdString.length());

    // Split/Convert command strings
    std::string path;
    for (int i = 0; i < cmdLength; i++) {
        const char c = pOptBuffer1[i];
        if (c == '#') {
            cmdVector.push_back(path);
            path.clear();
        } else {
            path.push_back(c);
        }
    }

    // Init paths
    std::string objModifierPath = cmdVector[0];
    std::string objFilePath = cmdVector[1];

    objModifierPath.erase(0, 2);
    objFilePath.erase(0, 2);

#ifdef _WIN32
    objModifierPath.insert(0, "\"C:");
    objFilePath.insert(0, "C:");
#else // MacOS
    objModifierPath.insert(0, "\"");
#endif
    objModifierPath.append("\"");

    std::vector<std::string> texture_paths(cmdVector.begin() + 2, cmdVector.end());

    // log path
    std::filesystem::path cmdLog = objFilePath;
    cmdLog.replace_filename("dspImporter.log");
    std::filesystem::path pluginLog = objFilePath;
    pluginLog.replace_filename("objModifier.log");

    std::ofstream logOfs(cmdLog, std::ios::out);
    if (!logOfs) {
        strcpy(pOptBuffer2, "Failed to open the log file.");
        return 1.0;
    } 

    // Run command setup
    std::stringstream cmdstream;
    cmdstream << objModifierPath << " -i " << objFilePath << " -o dspImporter_from_DLL -t";
    for (std::string& tex : texture_paths) {
        cmdstream << " " << tex;
    }
#ifdef __APPLE__
    cmdstream << " > " << pluginLog;
#endif
    
    logOfs << cmdstream.str() << std::endl;
    logOfs.close();
    
    auto ret = system(nullptr);
    if (ret == 0) {
        strcpy(pOptBuffer2, "Shell not available");
        return 1.0;
    }

    int r = system(cmdstream.str().c_str());
    if (r != 0) {
        // not successful
        strcpy(pOptBuffer2, "Failed to run the plugin command. Check the log.");
        return 1.0;
    }

    return 0.0f;
}


#ifdef _WIN32
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            // attach to process
            // return FALSE to fail DLL load
            break;

        case DLL_PROCESS_DETACH:
            // detach from process
            break;

        case DLL_THREAD_ATTACH:
            // attach to thread
            break;

        case DLL_THREAD_DETACH:
            // detach from thread
            break;
    }
    return TRUE; // succesful
}
#endif
