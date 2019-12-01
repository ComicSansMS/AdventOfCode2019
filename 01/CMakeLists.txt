add_library(01_rocket_equation STATIC rocket_equation.hpp rocket_equation.cpp)
target_include_directories(01_rocket_equation PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(01_rocket_equation PUBLIC range-v3)
add_executable(advent01 advent01.cpp)
target_link_libraries(advent01 PUBLIC 01_rocket_equation)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(01_rocket_equation_test rocket_equation.t.cpp)
    target_link_libraries(01_rocket_equation_test PUBLIC Catch 01_rocket_equation)
    add_test(NAME TestAdvent01 COMMAND 01_rocket_equation_test)
    enable_coverage(01_rocket_equation)
endif()
