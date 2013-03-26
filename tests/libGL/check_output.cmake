macro(EXEC_CHECK RES)
    execute_process(
        COMMAND ${PROGRAMM}
        OUTPUT_VARIABLE CMD_RES
        RESULT_VARIABLE EXIT_CODE
    )
    set(${RES} ${CMD_RES})

    if(NOT(${EXIT_CODE} EQUAL 0))
        message(FATAL_ERROR "Error running ${PROGRAMM} LD_LIBRARY_PATH='$ENV{LD_LIBRARY_PATH}'")
    else()
        message("${PROGRAMM} => ${EXIT_CODE}")
    endif()
endmacro()

# Normal run
set(ENV{LD_LIBRARY_PATH} "")
exec_check(${PROGRAMM} RES_ORIG)

# LD_LIBRARY_PATH is set
set(ENV{LD_LIBRARY_PATH} ${LIB_PATH})
exec_check(${PROGRAMM} RES_PROXIED)

string(COMPARE NOTEQUAL "${RES_PROXIED}" "${RES_ORIG}" RES)
if(${RES})
    message(FATAL_ERROR "Error: results are different")
endif()
