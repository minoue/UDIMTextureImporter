#ifndef displacementImporter_hpp
#define displacementImporter_hpp

#ifdef _WIN32
#include <windows.h>
#endif

// how to make a dylib
// https://developer.apple.com/library/archive/documentation/DeveloperTools/Conceptual/DynamicLibraries/100-Articles/CreatingDynamicLibraries.html#//apple_ref/doc/uid/TP40002073-SW1
//

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#endif

#ifdef __APPLE__
#define EXPORT __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

float EXPORT ImportVectorDisplacement(char* textFromZBrush, double zversion, char* pOptBuffer1, int optBuffer1Size,
    char* pOptBuffer2, int optBuffer2Size, char** zData);

#ifdef __cplusplus
}
#endif

#endif /* displacementImporter_hpp */
