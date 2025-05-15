set(TARGET pgmodeler-se)

# By default, the AUTOUIC option makes the uic look for the .ui files
# under the same folder as the sources. In this case, we force the uic
# command to look for .ui files under ui/*
set(CMAKE_AUTOUIC_SEARCH_PATHS ui)

qt_add_executable(${TARGET} WIN32 MACOSX_BUNDLE
    src/aboutsewidget.cpp src/aboutsewidget.h
    src/main.cpp
    src/schemaeditorform.cpp src/schemaeditorform.h
    src/sourceeditorwidget.cpp src/sourceeditorwidget.h
    ui/aboutsewidget.ui
    ui/schemaeditorform.ui
    ui/sourceeditorwidget.ui
    ../pgmodeler/src/pgmodelerapp.cpp)

target_include_directories(${TARGET} PRIVATE
    ${LIBCANVAS_INC}
    ${LIBCLI_INC}
    ${LIBCONNECTOR_INC}
    ${LIBCORE_INC}
    ${LIBGUI_INC}
    ${LIBPARSERS_INC}
    ${LIBUTILS_INC}
    ${TARGET}_autogen
    ../pgmodeler/src)

target_link_libraries(${TARGET} PRIVATE
    canvas
    cli
    connector
    core
    gui
    parsers
    utils)

pgm_install_executable(${TARGET} ON)
