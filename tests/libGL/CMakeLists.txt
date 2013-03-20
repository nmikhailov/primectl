set(TEST_INPUT    "${CMAKE_CURRENT_SOURCE_DIR}/test1.in.h")
set(TEST_OUTPUT   "${CMAKE_CURRENT_SOURCE_DIR}/test1.out.def")

set(TEST_TOOL_COMPARE "diff -w")

add_test(
    NAME GLDefGenerator
    COMMAND bash -c "${TEST_TOOL_COMPARE} ${TEST_OUTPUT} <(${TOOL_DEF_GENERATOR} ${TEST_INPUT})"
)
