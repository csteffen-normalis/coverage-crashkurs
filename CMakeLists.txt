cmake_minimum_required(VERSION 3.10)

project(coverage_demo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(--coverage -O0 -g)
add_link_options(--coverage)

include_directories(include)

enable_testing()