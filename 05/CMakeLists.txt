add_library(05_integer_program_mk2 STATIC integer_program_mk2.hpp integer_program_mk2.cpp)
target_include_directories(05_integer_program_mk2 PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(05_integer_program_mk2 PUBLIC range-v3)
add_executable(advent05 advent05.cpp)
target_link_libraries(advent05 PUBLIC 05_integer_program_mk2)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(05_integer_program_mk2_test integer_program_mk2.t.cpp)
    target_link_libraries(05_integer_program_mk2_test PUBLIC Catch 05_integer_program_mk2)
    add_test(NAME TestAdvent05 COMMAND 05_integer_program_mk2_test)
    enable_coverage(05_integer_program_mk2)
endif()
