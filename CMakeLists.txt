#  Copyright (c) 2021-2023 Daniel Volk <mail@volkarts.com>
#
#  This work is licensed under the terms of the MIT license.
#  For a copy, see LICENSE or <https://opensource.org/licenses/MIT>.

cmake_minimum_required(VERSION 3.16)

project(LockScreenTimeout
    VERSION 1.1
    LANGUAGES CXX
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

include(CompilerWarnings)
include(PreventInSourceBuilds)
include(QtSettings)

add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_20)
set_project_warnings(project_options)
apply_qt_settings(project_options)

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

find_package(Qt5 REQUIRED COMPONENTS Gui DBus X11Extras)
find_package(XCB REQUIRED COMPONENTS XCB DPMS)

if (${CMAKE_BUILD_TYPE} STREQUAL "DEBUG")
    add_definitions(-DLOGGING_TRACE_ENABLED)
endif()

add_subdirectory(src)
