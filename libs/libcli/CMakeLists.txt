set(TARGET cli)

set(CMAKE_AUTOUIC OFF)
set(CMAKE_AUTOMOC ON)

add_library(${TARGET} SHARED
    src/cliglobal.h
    src/compat/compatns.cpp src/compat/compatns.h
    src/compat/reference.cpp src/compat/reference.h
    src/compat/view.cpp src/compat/view.h
    src/pgmodelercliapp.cpp src/pgmodelercliapp.h
    src/pgmodelercliplugin.cpp src/pgmodelercliplugin.h)

target_include_directories(${TARGET} PUBLIC
    ${LIBCANVAS_INC}
    ${LIBCONNECTOR_INC}
    ${LIBCORE_INC}
    ${LIBGUI_INC}
    ${LIBPARSERS_INC}
    ${LIBUTILS_INC}
    src/compat)

target_link_libraries(${TARGET} PUBLIC
    canvas
    connector
    core
    gui
    parsers
    utils)

install(TARGETS ${TARGET}
    LIBRARY DESTINATION ${PGM_PRIVATELIBDIR}
    FRAMEWORK DESTINATION ${PGM_PRIVATELIBDIR}
    RUNTIME DESTINATION ${PGM_PRIVATELIBDIR})
