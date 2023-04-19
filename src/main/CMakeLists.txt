#  Copyright (c) 2021 Daniel Volk <mail@volkarts.com>
#
#  This work is licensed under the terms of the MIT license.
#  For a copy, see LICENSE or <https://opensource.org/licenses/MIT>.

add_executable(LockScreenTimeout "")

target_sources(LockScreenTimeout PRIVATE
    DisplayManager.cpp
    DisplayManager.h
    Dpms.cpp
    Dpms.h
    Logging.cpp
    Logging.h
    Main.cpp
    SessionListener.cpp
    SessionListener.h
    SessionState.h
    XCBDpms.cpp
    XCBDpms.h
)

target_link_libraries(LockScreenTimeout PRIVATE
    project_options
    Qt5::Gui
    Qt5::DBus
    Qt5::X11Extras
    ${XCB_LIBRARIES}
)

set_target_properties(LockScreenTimeout PROPERTIES
    RUNTIME_OUTPUT_NAME "lockscreentimeout"
)
