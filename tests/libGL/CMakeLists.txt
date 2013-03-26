# Packages
find_package(PNG REQUIRED)
find_package(Cairo REQUIRED)

# Debug builds
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")

add_test(
    NAME LibGL.export
    COMMAND ${CMAKE_COMMAND}
            -DLIB_MY=$<TARGET_FILE:GL>
            -DLIB_ORIG=${OPENGL_gl_LIBRARY}
            -P ${CMAKE_CURRENT_SOURCE_DIR}/check_export.cmake
)

# Common
add_library(test_common "test_common.c")
target_link_libraries(test_common ${X11_LIBRARIES} ${PNG_LIBRARIES})

macro(TEST_LIBGL NAME SOURCE LIBS)
    add_executable(test_${NAME} ${SOURCE})
    target_link_libraries(test_${NAME} ${LIBS})
    add_test(
        NAME LibGL.${NAME}
        COMMAND ${CMAKE_COMMAND}
                -DPROGRAMM=$<TARGET_FILE:test_${NAME}>
                -DLIB_PATH=$<TARGET_FILE_DIR:GL>
                -P ${CMAKE_CURRENT_SOURCE_DIR}/check_output.cmake
    )
endmacro()

test_libgl(linked "test_linked.c" "${OPENGL_LIBRARIES};test_common")
test_libgl(dlopen "test_dlopen.c" "dl;test_common")
test_libgl(cairo "test_cairo.c" "${CAIRO_LIBRARIES}")
