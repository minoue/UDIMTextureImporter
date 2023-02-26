#ifndef displacementImporter_hpp
#define displacementImporter_hpp

// how to make a dylib
// https://developer.apple.com/library/archive/documentation/DeveloperTools/Conceptual/DynamicLibraries/100-Articles/CreatingDynamicLibraries.html#//apple_ref/doc/uid/TP40002073-SW1
//

#ifdef __cplusplus
extern "C" {
#endif


float ImportVectorDisplacement(char* textFromZBrush, double zversion, char* pOptBuffer1, int optBuffer1Size,
    char* pOptBuffer2, int optBuffer2Size, char** zData);

#ifdef __cplusplus
}
#endif

#endif /* displacementImporter_hpp */
