set(PRIMED_DIR "${CMAKE_SOURCE_DIR}/src/primed/")

include_directories(${PRIMED_DIR})

set(TEST_SRC
    test_main.cpp
    utils_test.cpp
)

add_executable(primed_test ${TEST_SRC})
target_link_libraries(primed_test primed_base ${GTEST_BOTH_LIBRARIES})
gtest_add_tests(primed_test "" ${TEST_SRC})
