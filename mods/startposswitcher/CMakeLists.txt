cmake_minimum_required(VERSION 3.20)

add_library(startposswitcher_mod SHARED main.cpp)

set_target_properties(startposswitcher_mod PROPERTIES
    OUTPUT_NAME "startposswitcher"
    PREFIX ""
)

target_include_directories(startposswitcher_mod PRIVATE
    ${CMAKE_SOURCE_DIR}/loader/include
)
