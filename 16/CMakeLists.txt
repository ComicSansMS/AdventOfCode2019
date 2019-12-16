add_library(16_frequency_transmission STATIC frequency_transmission.hpp frequency_transmission.cpp)
target_include_directories(16_frequency_transmission PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(16_frequency_transmission PUBLIC range-v3)
add_executable(advent16 advent16.cpp)
target_link_libraries(advent16 PUBLIC 16_frequency_transmission)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(16_frequency_transmission_test frequency_transmission.t.cpp)
    target_link_libraries(16_frequency_transmission_test PUBLIC Catch 16_frequency_transmission)
    add_test(NAME TestAdvent16 COMMAND 16_frequency_transmission_test)
    enable_coverage(16_frequency_transmission)
endif()
