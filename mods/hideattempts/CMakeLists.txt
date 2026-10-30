cmake_minimum_required(VERSION 3.20)

add_library(hideattempts_mod SHARED main.cpp)

set_target_properties(hideattempts_mod PROPERTIES
    OUTPUT_NAME "hideattempts"
    PREFIX ""
)

target_include_directories(hideattempts_mod PRIVATE
    ${CMAKE_SOURCE_DIR}/loader/include
)
