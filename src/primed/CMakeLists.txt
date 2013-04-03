# Source list
set(PRIMED_BASE_SRC
    "client_matcher.cpp"
    "client_pool.cpp"
    "gpu_switch.cpp"
    "process_tracker.cpp"
    "settings.cpp"
    "xserverctl.cpp"
    "utils.cpp"
)

set(PRIMED_SRC
    "main.cpp"
    "dbus_server.cpp"
)

# primed
set(FILE_GEN_DBUS_ADAPTOR "${CMAKE_CURRENT_BINARY_DIR}/dbus_adaptor.hpp")

add_custom_command (
    OUTPUT ${FILE_GEN_DBUS_ADAPTOR}

    COMMAND dbusxx-xml2cpp "${FILE_DBUS_INTROSPECT}" --adaptor=${FILE_GEN_DBUS_ADAPTOR} 2> /dev/null
    COMMAND ${TOOL_WRAP_NOWARN} ${FILE_GEN_DBUS_ADAPTOR}

    DEPENDS ${FILE_DBUS_INTROSPECT}
)

include_directories(${DBUSCPP_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

add_library(primed_base ${PRIMED_BASE_SRC})

add_executable(primed ${PRIMED_SRC} ${FILE_GEN_DBUS_ADAPTOR})
target_link_libraries(primed primed_base ${DBUSCPP_LIBRARIES})
