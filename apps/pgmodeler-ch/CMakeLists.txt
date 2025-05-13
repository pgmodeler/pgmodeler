qt_add_executable(pgmodeler-ch WIN32 MACOSX_BUNDLE
    src/crashhandlerform.cpp src/crashhandlerform.h
    src/main.cpp)

target_include_directories(pgmodeler-ch PRIVATE
    ${LIBCANVAS_INC}
    ${LIBCLI_INC}
    ${LIBCONNECTOR_INC}
    ${LIBCORE_INC}
    ${LIBGUI_INC}
    ${LIBPARSERS_INC}
    ${LIBUTILS_INC}
    ../../plugins/src)

target_link_libraries(pgmodeler-ch PRIVATE
    canvas
    cli
    connector
    core
    gui
    parsers
    utils)

install(TARGETS pgmodeler-ch
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${PGM_PRIVATEBINDIR})
