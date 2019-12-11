add_library(11_intcode_robot STATIC intcode_robot.hpp intcode_robot.cpp)
target_include_directories(11_intcode_robot PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(11_intcode_robot PUBLIC range-v3 09_integer_program_mk3)
add_executable(advent11 advent11.cpp)
target_link_libraries(advent11 PUBLIC 11_intcode_robot)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(11_intcode_robot_test intcode_robot.t.cpp)
    target_link_libraries(11_intcode_robot_test PUBLIC Catch 11_intcode_robot)
    add_test(NAME TestAdvent11 COMMAND 11_intcode_robot_test)
    enable_coverage(11_intcode_robot)
endif()
