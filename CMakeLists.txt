cmake_minimum_required(VERSION 3.13)
project(displacementImporterGlobal)

add_subdirectory(DisplacementImporterData)

if (WIN32)
    set(TARGET_TYPE RUNTIME)
else()
    set(TARGET_TYPE LIBRARY)
endif()

install(TARGETS "displacementImporter" ${TARGET_TYPE} DESTINATION ../DisplacementImporterData)
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR})
