cmake_minimum_required(VERSION 2.8)
project(displacementImporter)

set(SOURCE_FILES
    displacementImporter.cpp
)
set(TARGET_TYPE RUNTIME)
set(INSTALL_DIR ../)
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

install(TARGETS ${PROJECT_NAME} ${TARGET_TYPE} DESTINATION ${INSTALL_DIR})
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR})
