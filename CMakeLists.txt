cmake_minimum_required(VERSION 3.26)
project(DAE_Galaga)

# Enforce a unified output directory for all executables & libraries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# Global Compile Features
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(EMSCRIPTEN)
  message(STATUS "Build Environment: Emscripten (Web)")
else()
  message(STATUS "Build Environment: Native")
endif()

# Static Analysis Global Export
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 1. Resolve External Dependencies
include(cmake/Dependencies.cmake)

# 2. Define Engine Framework
add_subdirectory(Minigin)

# 3. Define Gameplay Consumer
add_subdirectory(Game)
