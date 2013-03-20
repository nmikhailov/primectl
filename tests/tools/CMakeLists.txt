set(TEST_TOOL_COMPARE "diff -w")

MACRO(TEST_DEF_GENERATOR TESTNAME ARGS)
    set(TEST_INPUT    "${CMAKE_CURRENT_SOURCE_DIR}/${TESTNAME}.in.h")
    set(TEST_OUTPUT   "${CMAKE_CURRENT_SOURCE_DIR}/${TESTNAME}.out.def")

    add_test(
        NAME GLDefGenerator.${TESTNAME}
        COMMAND bash -c "${TEST_TOOL_COMPARE} ${TEST_OUTPUT} <(${TOOL_DEF_GENERATOR} ${ARGS} ${TEST_INPUT})"
    )
ENDMACRO(TEST_DEF_GENERATOR)

test_def_generator("test1" "")
test_def_generator("test2" "-e")
