set(SOURCES
  test_stack_operations.c
)

foreach(test ${SOURCES})
    set(test_name ${test}_ctest)
    add_executable(${test_name} ${test})
    target_link_libraries(${test_name} Forth-embed)
    add_test(${test_name} ${test_name})
endforeach()