# CMakeList.txt : CMake project for EcsTemplate, include source and define
# project specific logic here.
#

message("CMake version: ${CMAKE_VERSION}")
cmake_minimum_required (VERSION 3.18)

project ("EcsTemplate" VERSION 0.1)
message(STATUS "Project version: ${PROJECT_VERSION}")

set(CMAKE_CXX_STANDARD 23)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

if (POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

message("\nInlining CMake scripts")
# Project specific CMake scripts
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
message("CMake module paths: ${CMAKE_MODULE_PATH}")

# Embedded CMake scripts
include(CMakePrintHelpers)
include(FetchContent)
# Local CMake scripts
include(BuildInfo)

# spdlog
FetchContent_Declare(
    spdlog
    URL "${CMAKE_CURRENT_SOURCE_DIR}/external/spdlog-1.14.1.zip"
)
FetchContent_MakeAvailable(spdlog)
# spdlog end

# Add source to this project's executable.
add_executable (EcsTemplate "EcsTemplate.cpp" "EcsTemplate.h")
BuildInfo(EcsTemplate)
target_link_libraries(EcsTemplate PRIVATE spdlog::spdlog)
