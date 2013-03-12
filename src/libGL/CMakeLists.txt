# Source list
set(
    LIBGL_SRC

    "defs.c"
    "libGL.c"
    "utils.c"
)

# Generate function definitions from opengl headers
set(FILE_GEN_GL_DEF "${CMAKE_CURRENT_BINARY_DIR}/functions.def")

add_custom_command (
    OUTPUT ${FILE_GEN_GL_DEF}
    COMMAND ${TOOL_DEF_GENERATOR} ${OPENGL_INCLUDE_DIR}/GL/glx.h > ${FILE_GEN_GL_DEF}
    DEPENDS ${TOOL_DEF_GENERATOR}
)

# libGL
include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_library(GL SHARED ${LIBGL_SRC} ${FILE_GEN_GL_DEF})
target_link_libraries(GL dl)
