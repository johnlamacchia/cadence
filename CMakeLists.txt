cmake_minimum_required(VERSION 3.20)
project(cadence LANGUAGES C CXX ASM_MASM)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(loader)
add_subdirectory(mods/example)
add_subdirectory(mods/noclip)
add_subdirectory(mods/speedhack)
add_subdirectory(mods/fpsdisplay)
add_subdirectory(mods/practicemusic)
add_subdirectory(mods/instantrestart)
