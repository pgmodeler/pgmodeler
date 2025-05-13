qt_add_library(cli SHARED
    src/cliglobal.h
    src/compat/compatns.cpp src/compat/compatns.h
    src/compat/reference.cpp src/compat/reference.h
    src/compat/view.cpp src/compat/view.h
    src/pgmodelercliapp.cpp src/pgmodelercliapp.h
    src/pgmodelercliplugin.cpp src/pgmodelercliplugin.h
)

target_include_directories(cli PUBLIC
    ${LIBCANVAS_INC}
    ${LIBCONNECTOR_INC}
    ${LIBCORE_INC}
    ${LIBGUI_INC}
    ${LIBPARSERS_INC}
    ${LIBUTILS_INC}
    ../../plugins/src
    src/compat
)

target_link_libraries(cli PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Network
    Qt::PrintSupport
    Qt::Svg
    Qt::Widgets
    canvas
    connector
    core
    gui
    parsers
    utils
)

install(TARGETS cli
    LIBRARY DESTINATION ${PRIVATELIBDIR}
    FRAMEWORK DESTINATION ${PRIVATELIBDIR}
    RUNTIME DESTINATION ${PRIVATELIBDIR}
)
