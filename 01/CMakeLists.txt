add_library(01_test STATIC test.hpp test.cpp)
target_include_directories(01_test PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(01_test PUBLIC range-v3)
add_executable(advent01 advent01.cpp)
target_link_libraries(advent01 PUBLIC 01_test)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(01_test_test test.t.cpp)
    target_link_libraries(01_test_test PUBLIC Catch 01_test)
    add_test(NAME TestAdvent01 COMMAND 01_test_test)
    enable_coverage(01_test)
endif()
