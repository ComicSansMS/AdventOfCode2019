add_library(09_integer_program_mk3 STATIC integer_program_mk3.hpp integer_program_mk3.cpp)
target_include_directories(09_integer_program_mk3 PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(09_integer_program_mk3 PUBLIC range-v3)
add_executable(advent09 advent09.cpp)
target_link_libraries(advent09 PUBLIC 09_integer_program_mk3)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(09_integer_program_mk3_test integer_program_mk3.t.cpp)
    target_link_libraries(09_integer_program_mk3_test PUBLIC Catch 09_integer_program_mk3)
    add_test(NAME TestAdvent09 COMMAND 09_integer_program_mk3_test)
    enable_coverage(09_integer_program_mk3)
endif()
