add_library(modmenu_mod MODULE main.cpp)
target_include_directories(modmenu_mod PRIVATE ${CMAKE_SOURCE_DIR}/loader/include)
set_target_properties(modmenu_mod PROPERTIES
    PREFIX ""
    OUTPUT_NAME "modmenu"
    SUFFIX ".dll"
)
