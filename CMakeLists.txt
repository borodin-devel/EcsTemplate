# CMakeList.txt : CMake project for EcsTemplate, include source and define
# project specific logic here.
#

message("CMake version: ${CMAKE_VERSION}")
cmake_minimum_required (VERSION 3.8)

project ("EcsTemplate" VERSION 0.1)
message(STATUS "Project version: ${PROJECT_VERSION}")

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
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

# Add source to this project's executable.
add_executable (EcsTemplate "EcsTemplate.cpp" "EcsTemplate.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET EcsTemplate PROPERTY CXX_STANDARD 23)
endif()

BuildInfo(EcsTemplate)

# TODO: Add tests and install targets if needed.
