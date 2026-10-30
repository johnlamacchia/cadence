cmake_minimum_required(VERSION 3.20)

add_library(instantrestart_mod SHARED main.cpp)

set_target_properties(instantrestart_mod PROPERTIES
    OUTPUT_NAME "instantrestart"
    PREFIX ""
)

target_include_directories(instantrestart_mod PRIVATE
    ${CMAKE_SOURCE_DIR}/loader/include
)
