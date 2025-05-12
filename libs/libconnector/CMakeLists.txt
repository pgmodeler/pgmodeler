qt_add_library(connector SHARED
    src/catalog.cpp src/catalog.h
    src/connection.cpp src/connection.h
    src/connectorglobal.h
    src/resultset.cpp src/resultset.h
)
target_include_directories(connector PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/libcore/src
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/libparsers/src
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/libutils/src
    ../../plugins/src
)

target_compile_definitions(connector PUBLIC
    BINDIR=\\\"\\\"
    BUILDDATE=\\\"\\\"
    BUILDNUM=\\\"\\\"
    CONFDIR=\\\"\\\"
    CONNECTOR_SYMBOLS
    DOCDIR=\\\"\\\"
    LANGDIR=\\\"\\\"
    PLUGINSDIR=\\\"\\\"
    PRIVATEBINDIR=\\\"\\\"
    QT_DISABLE_DEPRECATED_BEFORE=0x060400
    SAMPLESDIR=\\\"\\\"
    SCHEMASDIR=\\\"\\\"
)

target_link_libraries(connector PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Network
    Qt::PrintSupport
    Qt::Svg
    Qt::Widgets
)

if(UNIX OR windows)
    target_link_libraries(connector PUBLIC
        # Remove: L(${CMAKE_CURRENT_SOURCE_DIR}/libs/libcore)
        # Remove: L(${CMAKE_CURRENT_SOURCE_DIR}/libs/libparsers)
        # Remove: L(${CMAKE_CURRENT_SOURCE_DIR}/libs/libutils)
        core
        parsers
        utils
    )
endif()

if(CONFIG___contains___debug)
    target_compile_definitions(connector PUBLIC
        PGMODELER_DEBUG
    )
endif()

#if(NOT defined(NO_CHECK_CURR_VER,var))
#    target_compile_definitions(connector PUBLIC
#        CHECK_CURR_VER
#    )
#endif()

if(SNAPSHOT_BUILD___equals___ON)
    target_compile_definitions(connector PUBLIC
        SNAPSHOT_BUILD
    )
endif()

if(DEMO_VERSION___equals___ON)
    target_compile_definitions(connector PUBLIC
        DEMO_VERSION
    )
endif()

if(NO_UPDATE_CHECK___equals___ON)
    target_compile_definitions(connector PUBLIC
        NO_UPDATE_CHECK
    )
endif()

if(PRIVATE_PLUGINS___equals___ON)
    target_compile_definitions(connector PUBLIC
        PRIVATE_PLUGINS_SYMBOLS
    )
endif()

if(APPIMAGE_BUILD___equals___ON AND LINUX)
    target_compile_definitions(connector PUBLIC
        APPIMAGE_BUILD
    )
endif()

#if(CMAKE_BUILD_TYPE STREQUAL Debug AND LINUX)
#    target_compile_options(connector
#        -Wall -Wextra -Wuninitialized
#    )
#endif()

if(MACOS)
    set_target_properties(connector PROPERTIES
        MACOSX_BUNDLE FALSE
    )
endif()

install(TARGETS connector
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
