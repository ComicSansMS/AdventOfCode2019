add_library(03_wires_crossing STATIC wires_crossing.hpp wires_crossing.cpp)
target_include_directories(03_wires_crossing PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(03_wires_crossing PUBLIC range-v3)
add_executable(advent03 advent03.cpp)
target_link_libraries(advent03 PUBLIC 03_wires_crossing)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(03_wires_crossing_test wires_crossing.t.cpp)
    target_link_libraries(03_wires_crossing_test PUBLIC Catch 03_wires_crossing)
    add_test(NAME TestAdvent03 COMMAND 03_wires_crossing_test)
    enable_coverage(03_wires_crossing)
endif()
