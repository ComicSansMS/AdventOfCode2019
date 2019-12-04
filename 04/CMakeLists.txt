add_library(04_password_cracker STATIC password_cracker.hpp password_cracker.cpp)
target_include_directories(04_password_cracker PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(04_password_cracker PUBLIC range-v3)
add_executable(advent04 advent04.cpp)
target_link_libraries(advent04 PUBLIC 04_password_cracker)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(04_password_cracker_test password_cracker.t.cpp)
    target_link_libraries(04_password_cracker_test PUBLIC Catch 04_password_cracker)
    add_test(NAME TestAdvent04 COMMAND 04_password_cracker_test)
    enable_coverage(04_password_cracker)
endif()
