cmake_minimum_required(VERSION 3.20)
project(MY_MOD LANGUAGES CXX)

# The Cadence SDK: a checkout of this repo, or the sdk/ folder of the unzipped release. Either one has the
# folders cmake/, loader/ and generated/ in it.
#
# Building this folder where it is (template/ inside the SDK) needs nothing: the SDK is the parent folder.
# If you copied the project somewhere else, say where the SDK is:
#   cmake -B build -DCADENCE_SDK_DIR=C:/path/to/cadence        (or C:/path/to/unzipped/sdk)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../cmake/CadenceMod.cmake")
    set(cadence_default_sdk "${CMAKE_CURRENT_SOURCE_DIR}/..")
endif()
set(CADENCE_SDK_DIR "${cadence_default_sdk}" CACHE PATH "Path to the Cadence SDK")
if(NOT EXISTS "${CADENCE_SDK_DIR}/cmake/CadenceMod.cmake")
    message(FATAL_ERROR
        "CADENCE_SDK_DIR is not set to a Cadence SDK (got: '${CADENCE_SDK_DIR}').\n"
        "Pass the folder that contains cmake/CadenceMod.cmake: a checkout of the repo, or the sdk/ folder of the "
        "unzipped release:\n  cmake -B build -DCADENCE_SDK_DIR=C:/path/to/cadence")
endif()

include(${CADENCE_SDK_DIR}/cmake/CadenceMod.cmake)

# NAME, AUTHOR, DESCRIPTION, KEY and VERSION become the mod's metadata (shown by the mod menu).
cadence_add_mod(MY_MOD main.cpp
    NAME "My Mod" AUTHOR "you" VERSION "1.0"
    DESCRIPTION "What it does"
    KEY "K")
