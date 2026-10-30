cmake_minimum_required(VERSION 3.20)

add_library(anticheat_mod SHARED main.cpp)

set_target_properties(anticheat_mod PROPERTIES
    OUTPUT_NAME "anticheat"
    PREFIX ""
)

target_include_directories(anticheat_mod PRIVATE
    ${CMAKE_SOURCE_DIR}/loader/include
)
