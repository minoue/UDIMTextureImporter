cmake_minimum_required(VERSION 3.13)
project(UDIMTextureImporter)

set(DLL_VERSION 2.3.3)

set(SOURCE_FILES
    ../UDIMTextureImporterData/src/udimTextureImporter.cpp
    ../UDIMTextureImporterData/src/goz.cpp
    ../UDIMTextureImporterData/src/util.cpp
    ../UDIMTextureImporterData/src/image.cpp
    ../UDIMTextureImporterData/src/timer.cpp
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
    set(PLUGIN_SUFFIX ".dll")
else()
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fvisibility=hidden")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
    set(PLUGIN_SUFFIX ".lib")
    set(CMAKE_SHARED_LINKER_FLAGS "-dynamiclib, -current_version ${DLL_VERSION}")
    set(CMAKE_MACOSX_RPATH 1)
endif()

if (TIFF_INSTALL_DIR)
    set(TIFF_INCLUDE_DIR "${TIFF_INSTALL_DIR}/include")
    set(TIFF_LIB_DIR "${TIFF_INSTALL_DIR}/lib")
    message(STATUS "Tiff include dir : ${TIFF_INCLUDE_DIR}")
    message(STATUS "Tiff lib dir : ${TIFF_LIB_DIR}")

    include_directories(${TIFF_INCLUDE_DIR})
    link_directories(${TIFF_LIB_DIR})
else ()
    find_package(TIFF)
    if (TIFF_FOUND)
        message("Tiff include dir : ${TIFF_INCLUDE_DIR}")
        message("Tiff libraries : ${TIFF_LIBRARIES}")
        include_directories(${TIFF_INCLUDE_DIR})
    else ()
        message(NOTICE "Tiff package not found. Using custom libtiff dir")
        if (NOT TIFF_INSTALL_DIR)
            message(FATAL_ERROR "Tiff install dir must be set")
        endif()
    endif ()
endif()

set(INSTALL_DIR "../UDIMTextureImporterData")

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ${PLUGIN_SUFFIX})
if (TIFF_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE TIFF::TIFF)
else ()
    target_link_libraries(${PROJECT_NAME} PRIVATE tiff)
endif()

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ../UDIMTextureImporterData
        LIBRARY DESTINATION ../UDIMTextureImporterData)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR})
