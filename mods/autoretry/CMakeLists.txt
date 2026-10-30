cmake_minimum_required(VERSION 3.20)

add_library(autoretry_mod SHARED main.cpp)

set_target_properties(autoretry_mod PROPERTIES
    OUTPUT_NAME "autoretry"
    PREFIX ""
)

target_include_directories(autoretry_mod PRIVATE
    ${CMAKE_SOURCE_DIR}/loader/include
)
