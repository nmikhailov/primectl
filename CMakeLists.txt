cmake_minimum_required(VERSION 2.6)
project(GL)
set(LIBGL_DEFAULT "/usr/lib/libGL.so")

configure_file(
    "${PROJECT_SOURCE_DIR}/common/config.h.in"
    "${PROJECT_BINARY_DIR}/common/config.h"
)

add_custom_command (
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/libGL/functions.def
    COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/libGL
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/tools/gen.py /usr/include/GL/glx.h > ${CMAKE_CURRENT_BINARY_DIR}/libGL/functions.def
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/tools/gen.py
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -std=c99")
add_definitions(-D_GNU_SOURCE)
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}/libGL)

add_library(GL SHARED libGL/defs.c libGL/libGL.c libGL/utils.c libGL/functions.def)
target_link_libraries(GL dl)
