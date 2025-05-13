qt_add_executable(pgmodeler-cli MACOSX_BUNDLE
    src/main.cpp
)

target_include_directories(pgmodeler-cli PRIVATE
    ${LIBCANVAS_INC}
    ${LIBCLI_INC}
    ${LIBCONNECTOR_INC}
    ${LIBCORE_INC}
    ${LIBGUI_INC}
    ${LIBPARSERS_INC}
    ${LIBUTILS_INC}
    ../../plugins/src
)

target_link_libraries(pgmodeler-cli PRIVATE
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

qt_generate_deploy_app_script(
    TARGET pgmodeler-cli
    FILENAME_VARIABLE deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)

install(SCRIPT ${deploy_script})
