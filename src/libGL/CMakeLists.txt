# Source list
set(
    LIBGL_SRC

    "defs.c"
    "dbus.c"
    "libGL.c"
)

# Set default visibility
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -fvisibility=hidden -g")

# Generate function definitions from opengl headers
set(FILE_GEN_GL_DEF "${CMAKE_CURRENT_BINARY_DIR}/glx.def")

add_custom_command (
    OUTPUT ${FILE_GEN_GL_DEF}
    COMMAND ${TOOL_DEF_GENERATOR} ${OPENGL_gl_LIBRARY} > ${FILE_GEN_GL_DEF}
    DEPENDS ${TOOL_DEF_GENERATOR}
)

# libGL
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${DBUS_INCLUDE_DIRS})

add_library(GL SHARED ${LIBGL_SRC} ${FILE_GEN_GL_DEF})
target_link_libraries(GL dl ${DBUS_LIBRARIES})
set_target_properties(GL
    PROPERTIES
    VERSION 1.2.0
    SOVERSION 1
)
