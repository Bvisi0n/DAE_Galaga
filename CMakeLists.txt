cmake_minimum_required(VERSION 3.26)
project(DAE_Galaga)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(cmake/Dependencies.cmake)

# Keep clang-tidy AFTER the depencies so it kindly ignores them.
option(ENABLE_CLANG_TIDY "Enable static analysis with clang-tidy" ON)

if(ENABLE_CLANG_TIDY AND NOT EMSCRIPTEN)
	find_program(CLANG_TIDY_EXE NAMES "clang-tidy" "clang-tidy.exe")
	if(CLANG_TIDY_EXE)
		set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE};--config-file=${CMAKE_SOURCE_DIR}/.clang-tidy")
		message(STATUS "Static Analysis: clang-tidy enabled globally.")
	endif()
endif()

add_subdirectory(Minigin)
add_subdirectory(Game)
