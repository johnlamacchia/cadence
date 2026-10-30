cmake_minimum_required(VERSION 3.20)

add_library(pausecountdown_mod SHARED main.cpp)

set_target_properties(pausecountdown_mod PROPERTIES
    OUTPUT_NAME "pausecountdown"
    PREFIX ""
)

target_include_directories(pausecountdown_mod PRIVATE
    ${CMAKE_SOURCE_DIR}/loader/include
)
