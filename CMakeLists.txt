cmake_minimum_required(VERSION 3.20)
# Before project(): the loader is a Windows DLL (MSVC, MASM, windows.h), so on anything else say so now instead of
# failing later with a compiler error about windows.h.
if(NOT CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    message(FATAL_ERROR
        "Cadence builds only on Windows (Visual Studio 2022 and MASM): it produces Windows DLLs.\n"
        "On Linux / Steam Deck you do not build anything: download the release zip and run install.sh "
        "(see the README, Install > Linux). Geometry Dash runs through Proton as the same Windows binary.")
endif()

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

# The installer bundles the loader and every mod above that is not NOT_BUNDLED.
add_subdirectory(installer)

# Compiles every generated GD header and evaluates all their static_asserts
# (class sizes, member offsets). Not part of the default build:
#   cmake --build build --config Release --target gd_headers_check
add_library(gd_headers_check OBJECT EXCLUDE_FROM_ALL tests/gd_headers.cpp)
target_include_directories(gd_headers_check PRIVATE loader/include generated/include)
target_compile_options(gd_headers_check PRIVATE /W3 /bigobj /utf-8)

add_test(NAME gd_headers
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --config $<CONFIG> --target gd_headers_check)

# Hook engine tests (no GD required): ctest -C Release
enable_testing()
add_executable(hook_test tests/hook_test.cpp loader/src/hook.cpp)
target_include_directories(hook_test PRIVATE loader/src)
target_link_libraries(hook_test PRIVATE Zydis)
set_target_properties(hook_test PROPERTIES LINK_FLAGS "/INCREMENTAL:NO")
add_test(NAME hook_test COMMAND hook_test)

# Loads the real loader and every built mod in a scratch folder (no GD needed).
add_executable(loader_smoke tests/loader_smoke.cpp)
add_dependencies(loader_smoke loader)
# Installs, upgrades, uninstalls and restores into throwaway folders with the real installer exe (no GD needed).
add_test(NAME installer
    COMMAND powershell -NoProfile -ExecutionPolicy Bypass -File "${CMAKE_SOURCE_DIR}/tests/run_installer_test.ps1"
            -Installer "$<TARGET_FILE:CadenceInstaller>")

# tools/install.sh against throwaway Steam folder trees (native, Flatpak, extra library drive).
find_program(CADENCE_BASH bash HINTS "C:/Program Files/Git/bin" "C:/Program Files (x86)/Git/bin" NO_DEFAULT_PATH)
if(NOT CADENCE_BASH AND NOT WIN32)
    find_program(CADENCE_BASH bash)
endif()
if(CADENCE_BASH)
    add_test(NAME install_sh
        COMMAND "${CADENCE_BASH}" tests/test_install_sh.sh "${CMAKE_BINARY_DIR}" "$<TARGET_FILE:CadenceInstaller>"
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
    set_tests_properties(install_sh PROPERTIES ENVIRONMENT "CONFIG=$<CONFIG>")
endif()

add_test(NAME loader_smoke
    COMMAND powershell -NoProfile -File "${CMAKE_SOURCE_DIR}/tests/run_loader_smoke.ps1" -BuildDir "${CMAKE_BINARY_DIR}")
