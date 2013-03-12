# Source list
set(
    PRIMED_SRC

    "main.cpp"
    "dbus_server.cpp"
)

# primed
set(FILE_GEN_DBUS_ADAPTOR "${CMAKE_CURRENT_BINARY_DIR}/dbus_adaptor.hpp")
add_custom_command (
    OUTPUT ${FILE_GEN_DBUS_ADAPTOR}

    COMMAND dbusxx-xml2cpp "${COMMON_SRC_DIR}/dbus-introspect.xml" --adaptor=${FILE_GEN_DBUS_ADAPTOR}
    COMMAND ${TOOL_WRAP_NOUNUSED} ${FILE_GEN_DBUS_ADAPTOR}

    DEPENDS ${COMMON_SRC_DIR}/dbus-introspect.xml
)

include_directories(${DBUSCPP_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

add_executable(primed ${PRIMED_SRC} ${FILE_GEN_DBUS_ADAPTOR})
target_link_libraries(primed ${DBUSCPP_LIBRARIES})
