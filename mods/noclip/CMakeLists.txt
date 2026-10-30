cmake_minimum_required(VERSION 3.20)

add_library(noclip_mod SHARED main.cpp)

set_target_properties(noclip_mod PROPERTIES
    OUTPUT_NAME "noclip"
    PREFIX ""
)

target_include_directories(noclip_mod PRIVATE
    ${CMAKE_SOURCE_DIR}/loader/include
)
