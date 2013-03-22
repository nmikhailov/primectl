# Test export
set(TOOL_CHECK_EXPORT "${CMAKE_CURRENT_SOURCE_DIR}/check_export.sh")

add_test(
    NAME LibGL.export
    COMMAND ${TOOL_CHECK_EXPORT} ${OPENGL_gl_LIBRARY} $<TARGET_FILE:GL>
)


# Common
find_package(PNG REQUIRED)
add_library(test_common "test_common.c")
target_link_libraries(test_common ${X11_LIBRARIES} ${PNG_LIBRARIES})

# Test linked
add_executable(test_linked "test_linked.c")
target_link_libraries(test_linked ${OPENGL_LIBRARIES} test_common)
add_test(
    NAME LibGL.linked
    COMMAND $<TARGET_FILE:test_linked>
)

# Test dlopen
add_executable(test_dlopen "test_dlopen.c")
target_link_libraries(test_dlopen dl test_common)
