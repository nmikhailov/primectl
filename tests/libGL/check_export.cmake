macro(EXEC_CHECK FILE RES)
    if(NOT EXISTS ${FILE})
        message(FATAL_ERROR "File ${FILE} not found")
    endif()
    execute_process(
        COMMAND nm -C -D ${FILE}
        COMMAND awk "{ if($2 == \"T\" || $2 == \"i\") print $3 }"
        COMMAND sort
        OUTPUT_VARIABLE CMD_RES
        RESULT_VARIABLE EXIT_CODE
    )
    set(${RES} ${CMD_RES})
    if(${EXIT_CODE})
        message(FATAL_ERROR "Error running nm ${FILE}")
    endif()
endmacro()

exec_check(${LIB_MY} RES_MY)
exec_check(${LIB_ORIG} RES_ORIG)

if(NOT (${RES_MY} STREQUAL ${RES_ORIG}))
    message(FATAL_ERROR "Error: results are different")
endif()
