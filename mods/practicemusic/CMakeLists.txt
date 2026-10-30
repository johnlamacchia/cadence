cmake_minimum_required(VERSION 3.20)

add_library(practicemusic_mod SHARED main.cpp)

set_target_properties(practicemusic_mod PROPERTIES
    OUTPUT_NAME "practicemusic"
    PREFIX ""
)

target_include_directories(practicemusic_mod PRIVATE
    ${CMAKE_SOURCE_DIR}/loader/include
)
