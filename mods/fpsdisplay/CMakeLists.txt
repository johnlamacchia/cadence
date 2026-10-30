cmake_minimum_required(VERSION 3.20)

add_library(fpsdisplay_mod SHARED main.cpp)

set_target_properties(fpsdisplay_mod PROPERTIES
    OUTPUT_NAME "fpsdisplay"
    PREFIX ""
)

target_include_directories(fpsdisplay_mod PRIVATE
    ${CMAKE_SOURCE_DIR}/loader/include
)
