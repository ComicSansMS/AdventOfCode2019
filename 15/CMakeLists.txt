add_library(15_path_finder STATIC path_finder.hpp path_finder.cpp)
target_include_directories(15_path_finder PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(15_path_finder PUBLIC range-v3 09_integer_program_mk3)
add_executable(advent15 advent15.cpp)
target_link_libraries(advent15 PUBLIC 15_path_finder)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(15_path_finder_test path_finder.t.cpp)
    target_link_libraries(15_path_finder_test PUBLIC Catch 15_path_finder)
    add_test(NAME TestAdvent15 COMMAND 15_path_finder_test)
    enable_coverage(15_path_finder)
endif()
