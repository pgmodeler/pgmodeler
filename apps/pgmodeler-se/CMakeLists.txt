# By default, the AUTOUIC option makes the uic look for the .ui files
# under the same folder as the sources. In this case, we force the uic
# command to look for .ui files under ui/*
set(CMAKE_AUTOUIC_SEARCH_PATHS ui)

qt_add_executable(pgmodeler-se WIN32 MACOSX_BUNDLE
    src/aboutsewidget.cpp src/aboutsewidget.h
    src/main.cpp
    src/schemaeditorform.cpp src/schemaeditorform.h
    src/sourceeditorwidget.cpp src/sourceeditorwidget.h
    ui/aboutsewidget.ui
    ui/schemaeditorform.ui
    ui/sourceeditorwidget.ui
)

target_include_directories(pgmodeler-se PRIVATE
    ${LIBCANVAS_INC}
    ${LIBCLI_INC}
    ${LIBCONNECTOR_INC}
    ${LIBCORE_INC}
    ${LIBGUI_INC}
    ${LIBPARSERS_INC}
    ${LIBUTILS_INC}
    pgmodeler-se_autogen
    ../../plugins/src
    ../pgmodeler/src
)

target_link_libraries(pgmodeler-se PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Network
    Qt::PrintSupport
    Qt::Svg
    Qt::Widgets
    canvas
    cli
    connector
    core
    gui
    parsers
    utils
)

install(TARGETS pgmodeler-se
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(
    TARGET pgmodeler-se
    FILENAME_VARIABLE deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)

install(SCRIPT ${deploy_script})
