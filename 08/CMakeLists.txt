add_library(08_image_decoding STATIC image_decoding.hpp image_decoding.cpp)
target_include_directories(08_image_decoding PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(08_image_decoding PUBLIC range-v3 05_integer_program_mk2)
add_executable(advent08 advent08.cpp)
target_link_libraries(advent08 PUBLIC 08_image_decoding)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(08_image_decoding_test image_decoding.t.cpp)
    target_link_libraries(08_image_decoding_test PUBLIC Catch 08_image_decoding)
    add_test(NAME TestAdvent08 COMMAND 08_image_decoding_test)
    enable_coverage(08_image_decoding)
endif()
