cmake_minimum_required(VERSION 2.6)
project(primectl)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")

# Tools
set(TOOL_DEF_GENERATOR "${CMAKE_SOURCE_DIR}/tools/gen2.py")
set(TOOL_WRAP_NOWARN "${CMAKE_SOURCE_DIR}/tools/wrap.sh")

# Common
set(COMMON_SRC_DIR  "${CMAKE_SOURCE_DIR}/src/common/")
set(FILE_DBUS_INTROSPECT  "${COMMON_SRC_DIR}/dbus-introspect.xml")

# Build flags
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -Wall -pedantic -std=c99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -std=c++11")
add_definitions(-D_GNU_SOURCE)

# Packages
find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(DBusCpp REQUIRED)
find_package(Glib REQUIRED)
find_package(GIO REQUIRED)

# Testing
enable_testing()

# Subdirectories
add_subdirectory("src")
add_subdirectory("tests")
