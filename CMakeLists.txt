cmake_minimum_required(VERSION 3.13)
project(displacementImporterGlobal)

add_subdirectory(DisplacementImporterData)
add_subdirectory(objModifier)

install(TARGETS "displacementImporter" "objModifier"
        RUNTIME DESTINATION ../DisplacementImporterData
        LIBRARY DESTINATION ../DisplacementImporterData)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR})
