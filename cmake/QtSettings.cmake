#
# Common settings for Qt projects
#

function(apply_qt_settings project_name)
    option(DISABLE_QT_ASCII_CASTS "Disable casts from and to ascii strings in QT" ON)

    if(DISABLE_QT_ASCII_CASTS)
        target_compile_definitions(${project_name} INTERFACE
            QT_NO_CAST_FROM_ASCII
            QT_NO_CAST_TO_ASCII
        )
    endif()
endfunction()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
#set(CMAKE_AUTOGEN_VERBOSE 1)
