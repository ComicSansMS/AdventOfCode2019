cmake_minimum_required(VERSION 3.15)

project(AdventOfCode2019 CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(GENERATE_COVERAGE_INFO "If set, line coverage info will be generated from debug test runs." OFF)
if(GENERATE_COVERAGE_INFO)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
            message(FATAL_ERROR "Coverage collection only supported for Debug builds.")
        endif()
        find_program(GCOV_COMMAND NAMES gcov DOC "Command for running gcov.")
        if(NOT GCOV_COMMAND)
            message(FATAL_ERROR "gcov was not found.")
        endif()
        add_compile_options(--coverage)
        link_libraries(--coverage)
        add_custom_target(coverage COMMENT "Collecting gcov coverage")
    endif()
endif()

function(enable_coverage target)
    if(GENERATE_COVERAGE_INFO AND (CMAKE_CXX_COMPILER_ID STREQUAL "GNU"))
        get_property(sources TARGET ${target} PROPERTY SOURCES)
        set(cov_commands "")
        set(valid_extensions ".c;.cpp;.cxx")
        get_property(target_bindir TARGET ${target} PROPERTY BINARY_DIR)
        file(RELATIVE_PATH target_bindir_rel ${PROJECT_BINARY_DIR} ${target_bindir})
        foreach(source ${sources})
            get_filename_component(file_ext ${source} EXT)
            list(FIND valid_extensions ${file_ext} is_source)
            if(NOT (is_source EQUAL -1))
                get_filename_component(file_name ${source} NAME)
                list(APPEND cov_commands COMMAND ${GCOV_COMMAND} -o ${target_bindir_rel}/CMakeFiles/${target}.dir/${file_name}.gcno ${source})
            endif()
        endforeach()
        if(cov_commands)
            add_custom_target(TestCoverage_${target}
                ${cov_commands}
                WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
                COMMENT "Collecting gcov coverage for ${target}"
            )
            add_dependencies(coverage TestCoverage_${target})
        endif()
    endif()
endfunction()


find_path(CATCH_INCLUDE_DIR NAMES catch.hpp PATHS ${PROJECT_SOURCE_DIR}/external/catch)
add_library(Catch STATIC ${PROJECT_SOURCE_DIR}/external/catch/catch_main.cpp)
target_include_directories(Catch PUBLIC ${CATCH_INCLUDE_DIR})
target_compile_definitions(Catch PUBLIC $<$<CXX_COMPILER_ID:MSVC>:_SILENCE_CXX17_UNCAUGHT_EXCEPTION_DEPRECATION_WARNING>)

find_package(range-v3 REQUIRED HINTS ${PROJECT_SOURCE_DIR}/external/range-v3)

add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/W4>)
add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/permissive->)
add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/std:c++latest>)
add_compile_options($<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wall>)
add_compile_options($<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-pedantic>)
add_compile_options($<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wextra>)

if(MSVC)
    configure_file(${PROJECT_SOURCE_DIR}/run.cmd.in ${PROJECT_BINARY_DIR}/run.cmd COPYONLY)
endif()

enable_testing()

add_subdirectory(01)
add_subdirectory(02)
add_subdirectory(03)
add_subdirectory(04)
add_subdirectory(05)
add_subdirectory(06)
add_subdirectory(07)
add_subdirectory(08)
