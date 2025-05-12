qt_add_library(cli SHARED
    src/cliglobal.h
    src/compat/compatns.cpp src/compat/compatns.h
    src/compat/reference.cpp src/compat/reference.h
    src/compat/view.cpp src/compat/view.h
    src/pgmodelercliapp.cpp src/pgmodelercliapp.h
    src/pgmodelercliplugin.cpp src/pgmodelercliplugin.h
)
target_include_directories(cli PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/libcanvas/src
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/libconnector/src
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/libcore/src
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/libgui/src
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/libparsers/src
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/libutils/src
    ../../plugins/src
    src/compat
)

target_compile_definitions(cli PUBLIC
    BINDIR=\\\"\\\"
    BUILDDATE=\\\"\\\"
    BUILDNUM=\\\"\\\"
    CLI_SYMBOLS
    CONFDIR=\\\"\\\"
    DOCDIR=\\\"\\\"
    LANGDIR=\\\"\\\"
    PLUGINSDIR=\\\"\\\"
    PRIVATEBINDIR=\\\"\\\"
    QT_DISABLE_DEPRECATED_BEFORE=0x060400
    SAMPLESDIR=\\\"\\\"
    SCHEMASDIR=\\\"\\\"
)

target_link_libraries(cli PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Network
    Qt::PrintSupport
    Qt::Svg
    Qt::Widgets
)

if(PRIVATE_PLUGINS___equals___ON)
    target_sources(cli PUBLIC
        priv-plugins/src/privpluginsns.cpp priv-plugins/src/privpluginsns.h
    )

    target_compile_definitions(cli PUBLIC
        PRIVATE_PLUGINS_SYMBOLS
    )
endif()

if(UNIX OR windows)
    target_link_libraries(cli PUBLIC
        # Remove: L(${CMAKE_CURRENT_SOURCE_DIR}/libs/libcanvas)
        # Remove: L(${CMAKE_CURRENT_SOURCE_DIR}/libs/libconnector)
        # Remove: L(${CMAKE_CURRENT_SOURCE_DIR}/libs/libcore)
        # Remove: L(${CMAKE_CURRENT_SOURCE_DIR}/libs/libgui)
        # Remove: L(${CMAKE_CURRENT_SOURCE_DIR}/libs/libparsers)
        # Remove: L(${CMAKE_CURRENT_SOURCE_DIR}/libs/libutils)
        canvas
        connector
        core
        gui
        parsers
        utils
    )
endif()

if(CONFIG___contains___debug)
    target_compile_definitions(cli PUBLIC
        PGMODELER_DEBUG
    )
endif()

#if(NOT defined(NO_CHECK_CURR_VER,var))
#    target_compile_definitions(cli PUBLIC
#        CHECK_CURR_VER
#    )
#endif()

if(SNAPSHOT_BUILD___equals___ON)
    target_compile_definitions(cli PUBLIC
        SNAPSHOT_BUILD
    )
endif()

if(DEMO_VERSION___equals___ON)
    target_compile_definitions(cli PUBLIC
        DEMO_VERSION
    )
endif()

if(NO_UPDATE_CHECK___equals___ON)
    target_compile_definitions(cli PUBLIC
        NO_UPDATE_CHECK
    )
endif()

if(APPIMAGE_BUILD___equals___ON AND LINUX)
    target_compile_definitions(cli PUBLIC
        APPIMAGE_BUILD
    )
endif()

#if(CMAKE_BUILD_TYPE STREQUAL Debug AND LINUX)
#    target_compile_options(cli
#        -Wall -Wextra -Wuninitialized
#    )
#endif()

if(MACOS)
    set_target_properties(cli PROPERTIES
        MACOSX_BUNDLE FALSE
    )
endif()

install(TARGETS cli
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
