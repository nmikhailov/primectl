# Source list
set(
    LIBGL_SRC

    "defs.c"
    "libGL.c"
    "utils.c"
)

# Generate function definitions from opengl headers
set(FILE_GEN_GL_DEF "${CMAKE_CURRENT_BINARY_DIR}/glx.def")

add_custom_command (
    OUTPUT ${FILE_GEN_GL_DEF}
    COMMAND ${TOOL_DEF_GENERATOR} ${OPENGL_INCLUDE_DIR}/GL/glx.h > ${FILE_GEN_GL_DEF}
    DEPENDS ${TOOL_DEF_GENERATOR}
)

# Generate dbus interfaces
set(FILE_GEN_DBUS_NAME "${CMAKE_CURRENT_BINARY_DIR}/dbus_bindings")
set(FILES_GEN_DBUS "${FILE_GEN_DBUS_NAME}.c" "${FILE_GEN_DBUS_NAME}.h")

add_custom_command (
    OUTPUT ${FILES_GEN_DBUS}
    COMMAND gdbus-codegen --generate-c-code ${FILE_GEN_DBUS_NAME} ${FILE_DBUS_INTROSPECT}
    COMMAND ${TOOL_WRAP_NOWARN} ${FILES_GEN_DBUS}
    DEPENDS ${FILE_DBUS_INTROSPECT}
)
# libGL
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${GLIB_INCLUDE_DIRS} ${GIO_INCLUDE_DIRS})

add_library(GL SHARED ${LIBGL_SRC} ${FILE_GEN_GL_DEF} ${FILES_GEN_DBUS})
target_link_libraries(GL dl ${GLIB_LIBRARIES} ${GIO_LIBRARIES})
