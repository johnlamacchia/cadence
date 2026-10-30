cmake_minimum_required(VERSION 3.20)

# Fetch Zydis for x64 instruction decoding
include(FetchContent)
FetchContent_Declare(
    zydis
    GIT_REPOSITORY https://github.com/zyantific/zydis.git
    GIT_TAG        v4.1.0
    GIT_SHALLOW    TRUE
)
set(ZYDIS_BUILD_TOOLS OFF CACHE BOOL "" FORCE)
set(ZYDIS_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(zydis)

# Pre-assemble the MASM proxy stubs via custom command
set(PROXY_ASM "${CMAKE_CURRENT_SOURCE_DIR}/src/proxy.asm")
set(PROXY_OBJ "${CMAKE_CURRENT_BINARY_DIR}/proxy.obj")

add_custom_command(
    OUTPUT "${PROXY_OBJ}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}"
    COMMAND ml64 /c /nologo /Fo "${PROXY_OBJ}" "${PROXY_ASM}"
    DEPENDS "${PROXY_ASM}"
    COMMENT "Assembling proxy.asm"
    VERBATIM
)

# The proxy DLL (output named winmm.dll)
add_library(loader SHARED
    src/proxy.cpp
    src/loader.cpp
    src/hook.cpp
    src/crash.cpp
    "${PROXY_OBJ}"
)

set_source_files_properties("${PROXY_OBJ}" PROPERTIES
    EXTERNAL_OBJECT TRUE
    GENERATED TRUE
)

set_target_properties(loader PROPERTIES
    OUTPUT_NAME "winmm"
    PREFIX ""
)

target_link_libraries(loader PRIVATE Zydis dbghelp)
target_include_directories(loader PUBLIC include)
target_include_directories(loader PRIVATE src)

# Use the .def file for exports
set_target_properties(loader PROPERTIES
    LINK_FLAGS "/DEF:\"${CMAKE_CURRENT_SOURCE_DIR}/src/proxy.def\""
)
