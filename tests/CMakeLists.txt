find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

add_subdirectory("libGL")
add_subdirectory("primed")
