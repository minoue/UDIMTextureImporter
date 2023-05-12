#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "goz.hpp"
#include "udimTextureImporter.hpp"

int EXPORT ImportUDIM(char* GoZFilePath,
    double gamma,
    char* pOptBuffer1,
    int optBuffer1Size,
    char* pOptBuffer2,
    int optBuffer2Size,
    char** zData)
{
    std::string GoZPathStr(GoZFilePath);

    // If a path from zscript starts with '!:', delete them.
    if (GoZPathStr.c_str()[0] == '!') {
        GoZPathStr.erase(0, 2);
    }

    std::filesystem::path gozPath(GoZPathStr);

    // Check if GoZ file exists
    bool gozFileExist = std::filesystem::exists(gozPath);
    if (!gozFileExist) {
        std::string message;
        message = "Cannot find GoZ file : ";
        message.append(gozPath.string());
        strcpy(pOptBuffer2, message.c_str());
        return 1;
    }

    // Split/Convert the long texture path string to vector
    // pOptBuffer1 comes in this format:
    // "1#C:/path/image.1001.tif#C:/path/image.1002.tif#C:/path.image.1003.tif ....
    // The first element is the mode(1-4), and the second to the last is the texture paths.
    std::vector<std::string> texture_paths;
    std::string pathString(pOptBuffer1);
    int pathStringLength = static_cast<int>(pathString.length());

    std::string path;
    for (int i = 0; i < pathStringLength; i++) {
        const char c = pOptBuffer1[i];
        if (c == '#') {
            texture_paths.push_back(path);
            path.clear();
        } else {
            path.push_back(c);
        }
    }

    // Extract the mode value, which is the first element of the texture_paths vector
    std::string modeStr = texture_paths[0];
    int mode = std::stoi(modeStr);
    texture_paths.erase(texture_paths.begin());

    GoZ obj;
    obj.read(gozPath.string());

    if (mode == 1) {
        obj.importVectorDisplacement(texture_paths);
    } else if (mode == 2) {
        obj.importNormalDisplacement(texture_paths);
    } else if (mode == 3) {
        obj.importVertexColor(texture_paths, gamma);
    } else if (mode == 4) {
        obj.importMask(texture_paths);
    } else {
        // Not supported
        strcpy(pOptBuffer2, "Invalid mode number");
        return 1;
    }

    // Export modified GoZ file
    gozPath.replace_filename("dspImporter_from_DLL.GoZ");
    std::cout << "GoZ output path: " << gozPath.string() << std::endl;
    obj.write(gozPath.string());

    return 0;
}

#ifdef _WIN32
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason) {
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
