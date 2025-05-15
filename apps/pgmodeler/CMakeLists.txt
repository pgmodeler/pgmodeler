set(TARGET pgmodeler)

qt_add_executable(${TARGET} WIN32 MACOSX_BUNDLE
    src/main.cpp
    src/pgmodelerapp.cpp src/pgmodelerapp.h)

target_include_directories(${TARGET} PRIVATE
    ${LIBCANVAS_INC}
    ${LIBCLI_INC}
    ${LIBCONNECTOR_INC}
    ${LIBCORE_INC}
    ${LIBGUI_INC}
    ${LIBPARSERS_INC}
    ${LIBUTILS_INC})

target_link_libraries(${TARGET} PRIVATE
    canvas
    cli
    connector
    core
    gui
    parsers
    utils)

pgm_install_executable(${TARGET} OFF)
