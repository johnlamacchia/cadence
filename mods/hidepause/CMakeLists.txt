cmake_minimum_required(VERSION 3.20)

add_library(hidepause_mod SHARED main.cpp)

set_target_properties(hidepause_mod PROPERTIES
    OUTPUT_NAME "hidepause"
    PREFIX ""
)

target_include_directories(hidepause_mod PRIVATE
    ${CMAKE_SOURCE_DIR}/loader/include
)
