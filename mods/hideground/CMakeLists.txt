cmake_minimum_required(VERSION 3.20)

add_library(hideground_mod SHARED main.cpp)

set_target_properties(hideground_mod PROPERTIES
    OUTPUT_NAME "hideground"
    PREFIX ""
)

target_include_directories(hideground_mod PRIVATE
    ${CMAKE_SOURCE_DIR}/loader/include
)
