qt_add_executable(pgmodeler-cli MACOSX_BUNDLE
    src/main.cpp)

target_include_directories(pgmodeler-cli PRIVATE
    ${LIBCANVAS_INC}
    ${LIBCLI_INC}
    ${LIBCONNECTOR_INC}
    ${LIBCORE_INC}
    ${LIBGUI_INC}
    ${LIBPARSERS_INC}
    ${LIBUTILS_INC}
    ../../plugins/src)

target_link_libraries(pgmodeler-cli PRIVATE
    canvas
    cli
    connector
    core
    gui
    parsers
    utils)

install(TARGETS pgmodeler-cli
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${PGM_BINDIR})
