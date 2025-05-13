qt_add_executable(pgmodeler WIN32 MACOSX_BUNDLE
    src/main.cpp
    src/pgmodelerapp.cpp src/pgmodelerapp.h
)
target_include_directories(pgmodeler PRIVATE
    ${LIBCANVAS_INC}
    ${LIBCLI_INC}
    ${LIBCONNECTOR_INC}
    ${LIBCORE_INC}
    ${LIBGUI_INC}
    ${LIBPARSERS_INC}
    ${LIBUTILS_INC}
    ../../plugins/src
)

target_link_libraries(pgmodeler PRIVATE
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

install(TARGETS pgmodeler
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${BINDIR}
)

#qt_generate_deploy_app_script(
#    TARGET pgmodeler
#    FILENAME_VARIABLE deploy_script
#    NO_UNSUPPORTED_PLATFORM_ERROR
#)

#install(SCRIPT ${deploy_script})
