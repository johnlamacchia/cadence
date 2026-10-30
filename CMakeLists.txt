cmake_minimum_required(VERSION 3.20)
project(cadence LANGUAGES C CXX ASM_MASM)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(loader)
add_subdirectory(mods/example)
