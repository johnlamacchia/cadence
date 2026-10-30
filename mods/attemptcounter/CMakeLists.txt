cmake_minimum_required(VERSION 3.20)

add_library(attemptcounter_mod SHARED main.cpp)

set_target_properties(attemptcounter_mod PROPERTIES
    OUTPUT_NAME "attemptcounter"
    PREFIX ""
)

target_include_directories(attemptcounter_mod PRIVATE
    ${CMAKE_SOURCE_DIR}/loader/include
)
