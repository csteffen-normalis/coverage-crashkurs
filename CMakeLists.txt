cmake_minimum_required(VERSION 3.10)

project(coverage_demo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(--coverage -O0 -g)
add_link_options(--coverage)

include_directories(include)

add_library(coverage_demo
    src/mode_decision.cpp
)

add_executable(coverage_tests
    test/test_mode_decision.cpp
)

target_link_libraries(coverage_tests
    coverage_demo
    gtest
    gtest_main
    pthread
)

enable_testing()
add_test(NAME coverage_tests COMMAND coverage_tests)
