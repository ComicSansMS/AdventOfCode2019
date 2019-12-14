add_library(14_fuel_refinement STATIC fuel_refinement.hpp fuel_refinement.cpp)
target_include_directories(14_fuel_refinement PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(14_fuel_refinement PUBLIC range-v3)
add_executable(advent14 advent14.cpp)
target_link_libraries(advent14 PUBLIC 14_fuel_refinement)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(14_fuel_refinement_test fuel_refinement.t.cpp)
    target_link_libraries(14_fuel_refinement_test PUBLIC Catch 14_fuel_refinement)
    add_test(NAME TestAdvent14 COMMAND 14_fuel_refinement_test)
    enable_coverage(14_fuel_refinement)
endif()
