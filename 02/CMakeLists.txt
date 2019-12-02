add_library(02_integer_program STATIC integer_program.hpp integer_program.cpp)
target_include_directories(02_integer_program PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(02_integer_program PUBLIC range-v3)
add_executable(advent02 advent02.cpp)
target_link_libraries(advent02 PUBLIC 02_integer_program)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(02_integer_program_test integer_program.t.cpp)
    target_link_libraries(02_integer_program_test PUBLIC Catch 02_integer_program)
    add_test(NAME TestAdvent02 COMMAND 02_integer_program_test)
    enable_coverage(02_integer_program)
endif()
