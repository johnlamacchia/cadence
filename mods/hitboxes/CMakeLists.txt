cmake_minimum_required(VERSION 3.20)

add_library(hitboxes_mod SHARED main.cpp)

set_target_properties(hitboxes_mod PROPERTIES
    OUTPUT_NAME "hitboxes"
    PREFIX ""
)

target_include_directories(hitboxes_mod PRIVATE
    ${CMAKE_SOURCE_DIR}/loader/include
)
