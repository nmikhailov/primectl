cmake_minimum_required(VERSION 2.6)
project(primectl)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")

# Tools
set(TOOL_DEF_GENERATOR "${CMAKE_SOURCE_DIR}/tools/gendefs.sh")
set(TOOL_WRAP_NOWARN "${CMAKE_SOURCE_DIR}/tools/wrap.sh")

# Build flags
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -Wall -pedantic -std=c99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -std=c++11")
add_definitions(-D_GNU_SOURCE)

# Packages
find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(DBusCpp REQUIRED)
find_package(DBus REQUIRED)

# Testing
enable_testing()

# Subdirectories
add_subdirectory("src")
add_subdirectory("tests")
