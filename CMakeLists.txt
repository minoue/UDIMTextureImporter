cmake_minimum_required(VERSION 2.8)
project(displacementImporter)

set(SOURCE_FILES
    displacementImporter.cpp
)
set(CMAKE_CXX_STANDARD 17)

if (WIN32)
    if (MSVC_VERSION GREATER_EQUAL "1900")
        include(CheckCXXCompilerFlag)
        CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
        if (_cpp_latest_flag_supported)
            add_compile_options("/std:c++latest")
        endif()
    endif()
else()
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Release)
    endif()

    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()

set(TARGET_TYPE RUNTIME)
set(INSTALL_DIR ../DisplacementImporterData)
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

install(TARGETS ${PROJECT_NAME} ${TARGET_TYPE} DESTINATION ${INSTALL_DIR})
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR})
