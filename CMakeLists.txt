cmake_minimum_required(VERSION 2.6)
project(primectl)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")

# Tools
set(DEF_GENERATOR "${CMAKE_SOURCE_DIR}/tools/gen.py")

# Build flags
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -Wall -pedantic -std=c99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -std=c++11 -Wunused-but-set-variable")

add_definitions(-D_GNU_SOURCE)

# Source list
set(LIBGL_SRC_DIR   "${CMAKE_SOURCE_DIR}/src/libGL/")
set(PRIMED_SRC_DIR  "${CMAKE_SOURCE_DIR}/src/primed/")
set(COMMON_SRC_DIR  "${CMAKE_SOURCE_DIR}/src/common/")

set(LIBGL_SRC       "${LIBGL_SRC_DIR}/defs.c"
                    "${LIBGL_SRC_DIR}/libGL.c"
                    "${LIBGL_SRC_DIR}/utils.c"
)

set(PRIMED_SRC      "${PRIMED_SRC_DIR}/main.cpp"
                    "${PRIMED_SRC_DIR}/dbus_server.cpp"
)

# Packages
find_package(OpenGL REQUIRED)

# Generate function definitions from opengl headers
set(FILE_GEN_GL_DEF "${CMAKE_BINARY_DIR}/functions.def")
add_custom_command (
    OUTPUT ${FILE_GEN_GL_DEF}
    COMMAND ${DEF_GENERATOR} ${OPENGL_INCLUDE_DIR}/GL/glx.h > ${FILE_GEN_GL_DEF}
    DEPENDS ${DEF_GENERATOR}
)

# libGL
include_directories(${CMAKE_SOURCE_DIR}/src/ ${CMAKE_BINARY_DIR})
add_library(GL SHARED ${LIBGL_SRC} ${FILE_GEN_GL_DEF})
target_link_libraries(GL dl)

# primed

set(FILE_GEN_DBUS_ADAPTOR "${CMAKE_BINARY_DIR}/dbus_adaptor.hpp")
add_custom_command (
    OUTPUT ${FILE_GEN_DBUS_ADAPTOR}
    COMMAND dbusxx-xml2cpp "${COMMON_SRC_DIR}/dbus-introspect.xml" --adaptor=${FILE_GEN_DBUS_ADAPTOR}

    # Disable unused variable warnings in generated code
    #COMMAND echo "\"#pragma GCC diagnostic ignored \\\\"-Wunused-variable\\\\"\\n$(cat ${FILE_GEN_DBUS_ADAPTOR})\"" > ${FILE_GEN_DBUS_ADAPTOR}
    #COMMAND echo "\"#pragma GCC diagnostic pop\\n" >> ${FILE_GEN_DBUS_ADAPTOR}

    DEPENDS ${COMMON_SRC_DIR}/dbus-introspect.xml
)

find_package(DBusCpp REQUIRED)
include_directories(${DBUSCPP_INCLUDE_DIR})

add_executable(primed ${PRIMED_SRC} ${FILE_GEN_DBUS_ADAPTOR})
target_link_libraries(primed ${DBUSCPP_LIBRARIES})
