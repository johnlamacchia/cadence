cmake_minimum_required(VERSION 3.20)
project(cadence LANGUAGES C CXX ASM_MASM)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(loader)

# Every directory under mods/ that has a CMakeLists.txt is built as a mod.
include(cmake/CadenceMod.cmake)
file(GLOB mod_dirs CONFIGURE_DEPENDS LIST_DIRECTORIES true RELATIVE "${CMAKE_SOURCE_DIR}/mods" "${CMAKE_SOURCE_DIR}/mods/*")
foreach(dir ${mod_dirs})
    if(EXISTS "${CMAKE_SOURCE_DIR}/mods/${dir}/CMakeLists.txt")
        add_subdirectory(mods/${dir})
    endif()
endforeach()

# Hook engine tests (no GD required): ctest -C Release
enable_testing()
add_executable(hook_test tests/hook_test.cpp loader/src/hook.cpp)
target_include_directories(hook_test PRIVATE loader/src)
target_link_libraries(hook_test PRIVATE Zydis)
set_target_properties(hook_test PROPERTIES LINK_FLAGS "/INCREMENTAL:NO")
add_test(NAME hook_test COMMAND hook_test)
