# CMakeList.txt : CMake project for EcsTemplate, include source and define
# project specific logic here.
#

message(STATUS "Build information")
message("CMake version: ${CMAKE_VERSION}")
cmake_minimum_required(VERSION 3.18)

project("EcsTemplate" VERSION 0.1)
message("Project version: ${PROJECT_VERSION}")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Enable Hot Reload for MSVC compilers if supported.
if(POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

message("Project build type: ${CMAKE_BUILD_TYPE}")

if(MSVC)
    message("MSVC Version: ${MSVC_VERSION}")
endif()

message(STATUS "Inlining CMake scripts")

# Project specific CMake scripts
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
message("CMake module paths: ${CMAKE_MODULE_PATH}")

# Embedded CMake scripts
include(CMakePrintHelpers)
include(FetchContent)

# Local CMake scripts
include(buildinfo)

# Include sub-projects
message(STATUS "Fetching dependencies")

# spdlog
FetchContent_Declare(
    spdlog
    URL "${CMAKE_CURRENT_SOURCE_DIR}/external/spdlog-1.14.1.zip"
)
FetchContent_MakeAvailable(spdlog)

# spdlog end

# raylib
FetchContent_Declare(
    raylib
    URL "${CMAKE_CURRENT_SOURCE_DIR}/external/raylib-5.0.zip"
)

# set(raylib_VERBOSE 1)
set(raylib_BUILD_EXAMPLES OFF CACHE BOOL "Build Raylib examples" FORCE)
set(raylib_BUILD_GAMES OFF CACHE BOOL "Build Raylib games" FORCE)
FetchContent_MakeAvailable(raylib)

# raylib end

# raylib-cpp
FetchContent_Declare(
    raylib_cpp
    URL "${CMAKE_CURRENT_SOURCE_DIR}/external/raylib-cpp-5.0.2.zip"
)
FetchContent_MakeAvailable(raylib_cpp)

# raylib-cpp end
message(STATUS "Building the project")

# Add source to this project's executable.
add_executable(EcsTemplate
    "main.cpp"
    "src/globals.hpp"
    "src/globals.cpp"
    "src/logger.hpp"
    "src/logger.cpp"
    "src/console.hpp"
    "src/console.cpp"
    "src/platform/platform_base.hpp"
    "src/platform/platform_base.cpp"
    "src/platform/platform_windows.hpp"
    "src/platform/platform_windows.cpp"
)

target_compile_definitions(EcsTemplate PRIVATE $<$<CONFIG:Debug>:BUILD_TYPE_DEBUG>)

# Enable Unicode for Windows platforms
if(WIN32)
    target_compile_definitions(EcsTemplate PRIVATE UNICODE _UNICODE)
endif()

BuildInfo(EcsTemplate)

if(WIN32)
    target_link_libraries(EcsTemplate
        PRIVATE
        dbghelp
    )
endif()

target_link_libraries(EcsTemplate
    PRIVATE
    spdlog::spdlog
    raylib
    raylib_cpp
)
