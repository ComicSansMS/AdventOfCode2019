add_library(07_amp_settings STATIC amp_settings.hpp amp_settings.cpp)
target_include_directories(07_amp_settings PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(07_amp_settings PUBLIC range-v3 05_integer_program_mk2)
add_executable(advent07 advent07.cpp)
target_link_libraries(advent07 PUBLIC 07_amp_settings)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(07_amp_settings_test amp_settings.t.cpp)
    target_link_libraries(07_amp_settings_test PUBLIC Catch 07_amp_settings)
    add_test(NAME TestAdvent07 COMMAND 07_amp_settings_test)
    enable_coverage(07_amp_settings)
endif()
