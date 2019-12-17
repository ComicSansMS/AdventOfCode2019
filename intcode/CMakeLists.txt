add_library(intcode STATIC intcode.hpp intcode.cpp)
target_include_directories(intcode PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(intcode PRIVATE range-v3)

if(TARGET Catch)
    add_library(intcode_mock intcode_mock.cpp intcode_mock.hpp)
    target_include_directories(intcode_mock PUBLIC ${CMAKE_CURRENT_LIST_DIR})

    add_executable(intcode_test intcode.t.cpp)
    target_link_libraries(intcode_test PUBLIC Catch intcode)
    add_test(NAME TestIntcode COMMAND intcode_test)
    enable_coverage(intcode_test)
endif()
