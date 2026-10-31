cmake_minimum_required(VERSION 3.20)
project(MY_MOD LANGUAGES CXX)

# Path to the Cadence SDK: this repo, or the unzipped release's sdk/ folder.
set(CADENCE_SDK_DIR "" CACHE PATH "Path to the Cadence SDK")
if(NOT CADENCE_SDK_DIR)
    message(FATAL_ERROR "Pass -DCADENCE_SDK_DIR=<path to cadence>")
endif()

include(${CADENCE_SDK_DIR}/cmake/CadenceMod.cmake)

# NAME, AUTHOR, DESCRIPTION, KEY and VERSION become the mod's metadata (shown by the mod menu).
cadence_add_mod(MY_MOD main.cpp
    NAME "My Mod" AUTHOR "you" VERSION "1.0"
    DESCRIPTION "What it does"
    KEY "K")
