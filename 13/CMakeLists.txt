add_library(13_intcode_arcade STATIC intcode_arcade.hpp intcode_arcade.cpp)
target_include_directories(13_intcode_arcade PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(13_intcode_arcade PUBLIC range-v3 09_integer_program_mk3)
add_executable(advent13 advent13.cpp)
target_link_libraries(advent13 PUBLIC 13_intcode_arcade)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(13_intcode_arcade_test intcode_arcade.t.cpp)
    target_link_libraries(13_intcode_arcade_test PUBLIC Catch 13_intcode_arcade)
    add_test(NAME TestAdvent13 COMMAND 13_intcode_arcade_test)
    enable_coverage(13_intcode_arcade)
endif()
