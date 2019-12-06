add_library(06_planet_orbits STATIC planet_orbits.hpp planet_orbits.cpp)
target_include_directories(06_planet_orbits PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(06_planet_orbits PUBLIC range-v3)
add_executable(advent06 advent06.cpp)
target_link_libraries(advent06 PUBLIC 06_planet_orbits)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(06_planet_orbits_test planet_orbits.t.cpp)
    target_link_libraries(06_planet_orbits_test PUBLIC Catch 06_planet_orbits)
    add_test(NAME TestAdvent06 COMMAND 06_planet_orbits_test)
    enable_coverage(06_planet_orbits)
endif()
