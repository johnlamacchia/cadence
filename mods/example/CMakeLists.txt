cmake_minimum_required(VERSION 3.20)

add_library(example_mod SHARED main.cpp)

set_target_properties(example_mod PROPERTIES
    OUTPUT_NAME "example"
    PREFIX ""
)

target_include_directories(example_mod PRIVATE
    ${CMAKE_SOURCE_DIR}/loader/include
)
