cmake_minimum_required(VERSION 2.6)
project(primectl)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")

# Tools
set(TOOL_DEF_GENERATOR "${CMAKE_SOURCE_DIR}/tools/gen.py")
set(TOOL_WRAP_NOUNUSED "${CMAKE_SOURCE_DIR}/tools/wrap.sh")

# Build flags
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -Wall -pedantic -std=c99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -std=c++11")

add_definitions(-D_GNU_SOURCE)

# Packages
find_package(OpenGL REQUIRED)
find_package(DBusCpp REQUIRED)

#
set(COMMON_SRC_DIR  "${CMAKE_SOURCE_DIR}/src/common/")
include_directories("${CMAKE_SOURCE_DIR}/src/")

add_subdirectory("src/libGL")
add_subdirectory("src/primed")
