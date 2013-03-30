include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

# Common
set(FILE_DBUS_INTROSPECT "${CMAKE_CURRENT_SOURCE_DIR}/common/dbus-introspect.xml")

add_subdirectory("libGL")
add_subdirectory("primed")
