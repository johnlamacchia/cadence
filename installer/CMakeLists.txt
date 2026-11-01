# The installer: one CadenceInstaller.exe that carries the loader, the bundled mods and the
# address data inside itself (as a resource) and installs them into a Geometry Dash folder.
set(CADENCE_VERSION "1.1.1")

add_executable(cadence_pack pack.cpp)
set_target_properties(cadence_pack PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/tools")

# What goes inside: the loader, every mod built here that did not say NOT_BUNDLED, and the data files.
get_property(bundled_mods GLOBAL PROPERTY CADENCE_BUNDLED_MODS)
set(payload_lines "$<TARGET_FILE:loader>|winmm.dll\n")
foreach(mod_target ${bundled_mods})
    string(APPEND payload_lines "$<TARGET_FILE:${mod_target}>|mods/$<TARGET_FILE_NAME:${mod_target}>\n")
endforeach()
file(GLOB data_files CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/generated/addresses_*.json"
    "${CMAKE_SOURCE_DIR}/generated/layout_hashes_*.json")
foreach(data_file ${data_files})
    get_filename_component(data_name "${data_file}" NAME)
    if(NOT data_name MATCHES "^addresses_rejected_")      # a report of dropped addresses, not an address file
        string(APPEND payload_lines "${data_file}|generated/${data_name}\n")
    endif()
endforeach()
file(GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/payload_list_$<CONFIG>.txt" CONTENT "${payload_lines}")

add_custom_command(
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/payload.bin"
    COMMAND cadence_pack "${CMAKE_CURRENT_BINARY_DIR}/payload.bin" "${CMAKE_CURRENT_BINARY_DIR}/payload_list_$<CONFIG>.txt"
    DEPENDS cadence_pack loader ${bundled_mods} ${data_files} "${CMAKE_CURRENT_BINARY_DIR}/payload_list_$<CONFIG>.txt"
    COMMENT "Packing the installer payload"
    VERBATIM)

add_executable(CadenceInstaller WIN32 main.cpp installer.rc "${CMAKE_CURRENT_BINARY_DIR}/payload.bin")
set_source_files_properties(installer.rc PROPERTIES OBJECT_DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/payload.bin")
target_include_directories(CadenceInstaller PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
target_compile_definitions(CadenceInstaller PRIVATE CADENCE_VERSION="${CADENCE_VERSION}" UNICODE _UNICODE)
set_target_properties(CadenceInstaller PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/installer")
target_link_libraries(CadenceInstaller PRIVATE comctl32 shell32 ole32 advapi32 user32)
if(MSVC)
    target_compile_options(CadenceInstaller PRIVATE /W4 /utf-8)
    target_link_options(CadenceInstaller PRIVATE /MANIFESTUAC:NO)
endif()
