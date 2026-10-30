cmake_minimum_required(VERSION 3.20)

add_library(noglow_mod SHARED main.cpp)

set_target_properties(noglow_mod PROPERTIES
    OUTPUT_NAME "noglow"
    PREFIX ""
)

target_include_directories(noglow_mod PRIVATE
    ${CMAKE_SOURCE_DIR}/loader/include
)
