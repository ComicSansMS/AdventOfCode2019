add_library(10_asteroid_scanner STATIC asteroid_scanner.hpp asteroid_scanner.cpp)
target_include_directories(10_asteroid_scanner PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(10_asteroid_scanner PUBLIC range-v3)
add_executable(advent10 advent10.cpp)
target_link_libraries(advent10 PUBLIC 10_asteroid_scanner)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(10_asteroid_scanner_test asteroid_scanner.t.cpp)
    target_link_libraries(10_asteroid_scanner_test PUBLIC Catch 10_asteroid_scanner)
    add_test(NAME TestAdvent10 COMMAND 10_asteroid_scanner_test)
    enable_coverage(10_asteroid_scanner)
endif()
