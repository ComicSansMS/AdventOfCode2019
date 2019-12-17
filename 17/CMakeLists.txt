add_library(17_vacuum_robot STATIC vacuum_robot.hpp vacuum_robot.cpp)
target_include_directories(17_vacuum_robot PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(17_vacuum_robot PUBLIC range-v3 intcode)
add_executable(advent17 advent17.cpp)
target_link_libraries(advent17 PUBLIC 17_vacuum_robot)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(17_vacuum_robot_test vacuum_robot.t.cpp)
    target_link_libraries(17_vacuum_robot_test PUBLIC Catch intcode_mock 17_vacuum_robot)
    add_test(NAME TestAdvent17 COMMAND 17_vacuum_robot_test)
    enable_coverage(17_vacuum_robot)
endif()
