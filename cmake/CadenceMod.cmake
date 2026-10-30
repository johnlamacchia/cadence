# Build helper for Cadence mods. Works from this repo and from a standalone mod project:
#
#   set(CADENCE_SDK_DIR "path/to/cadence" CACHE PATH "")
#   include(${CADENCE_SDK_DIR}/cmake/CadenceMod.cmake)
#   cadence_add_mod(my_mod main.cpp)
#
# Produces my_mod.dll (target: my_mod_mod). Drop it in <GD>/mods/ and press F6 in game.

if(NOT CADENCE_SDK_DIR)
    get_filename_component(CADENCE_SDK_DIR "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)
endif()

function(cadence_add_mod name)
    add_library(${name}_mod MODULE ${ARGN})
    target_include_directories(${name}_mod PRIVATE "${CADENCE_SDK_DIR}/loader/include")
    target_compile_features(${name}_mod PRIVATE cxx_std_20)
    set_target_properties(${name}_mod PROPERTIES
        PREFIX ""
        OUTPUT_NAME "${name}"
        SUFFIX ".dll"
    )
endfunction()
