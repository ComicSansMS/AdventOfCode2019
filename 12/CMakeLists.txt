add_library(12_moon_simulator STATIC moon_simulator.hpp moon_simulator.cpp)
target_include_directories(12_moon_simulator PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(12_moon_simulator PUBLIC range-v3)
add_executable(advent12 advent12.cpp)
target_link_libraries(advent12 PUBLIC 12_moon_simulator)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(12_moon_simulator_test moon_simulator.t.cpp)
    target_link_libraries(12_moon_simulator_test PUBLIC Catch 12_moon_simulator)
    add_test(NAME TestAdvent12 COMMAND 12_moon_simulator_test)
    enable_coverage(12_moon_simulator)
endif()
