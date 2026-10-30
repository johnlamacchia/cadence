cmake_minimum_required(VERSION 3.20)

add_library(speedhack_mod SHARED main.cpp)

set_target_properties(speedhack_mod PROPERTIES
    OUTPUT_NAME "speedhack"
    PREFIX ""
)

target_include_directories(speedhack_mod PRIVATE
    ${CMAKE_SOURCE_DIR}/loader/include
)
