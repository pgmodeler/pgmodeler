qt_add_executable(pgmodeler WIN32 MACOSX_BUNDLE
    src/main.cpp
    src/pgmodelerapp.cpp src/pgmodelerapp.h)

target_include_directories(pgmodeler PRIVATE
    ${LIBCANVAS_INC}
    ${LIBCLI_INC}
    ${LIBCONNECTOR_INC}
    ${LIBCORE_INC}
    ${LIBGUI_INC}
    ${LIBPARSERS_INC}
    ${LIBUTILS_INC}
    ../../plugins/src)

target_link_libraries(pgmodeler PRIVATE
    canvas
    cli
    connector
    core
    gui
    parsers
    utils)

install(TARGETS pgmodeler
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${PGM_BINDIR})
