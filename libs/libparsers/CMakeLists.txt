qt_add_library(parsers SHARED
    src/attributes.h
    src/csvdocument.cpp src/csvdocument.h
    src/csvparser.cpp src/csvparser.h
    src/parsersglobal.h
    src/schemaparser.cpp src/schemaparser.h
    src/xmlparser.cpp src/xmlparser.h
)
target_include_directories(parsers PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/libutils/src
    ../../plugins/src
)

target_compile_definitions(parsers PUBLIC
    BINDIR=\\\"\\\"
    BUILDDATE=\\\"\\\"
    BUILDNUM=\\\"\\\"
    CONFDIR=\\\"\\\"
    DOCDIR=\\\"\\\"
    LANGDIR=\\\"\\\"
    PARSERS_SYMBOLS
    PLUGINSDIR=\\\"\\\"
    PRIVATEBINDIR=\\\"\\\"
    QT_DISABLE_DEPRECATED_BEFORE=0x060400
    SAMPLESDIR=\\\"\\\"
    SCHEMASDIR=\\\"\\\"
)

target_link_libraries(parsers PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Network
    Qt::PrintSupport
    Qt::Svg
    Qt::Widgets
)

if(UNIX OR windows)
    target_link_libraries(parsers PUBLIC
        # Remove: L(${CMAKE_CURRENT_SOURCE_DIR}/libs/libutils)
        utils
    )
endif()

if(CONFIG___contains___debug)
    target_compile_definitions(parsers PUBLIC
        PGMODELER_DEBUG
    )
endif()

#if(NOT defined(NO_CHECK_CURR_VER,var))
#    target_compile_definitions(parsers PUBLIC
#        CHECK_CURR_VER
#    )
#endif()

if(SNAPSHOT_BUILD___equals___ON)
    target_compile_definitions(parsers PUBLIC
        SNAPSHOT_BUILD
    )
endif()

if(DEMO_VERSION___equals___ON)
    target_compile_definitions(parsers PUBLIC
        DEMO_VERSION
    )
endif()

if(NO_UPDATE_CHECK___equals___ON)
    target_compile_definitions(parsers PUBLIC
        NO_UPDATE_CHECK
    )
endif()

if(PRIVATE_PLUGINS___equals___ON)
    target_compile_definitions(parsers PUBLIC
        PRIVATE_PLUGINS_SYMBOLS
    )
endif()

if(APPIMAGE_BUILD___equals___ON AND LINUX)
    target_compile_definitions(parsers PUBLIC
        APPIMAGE_BUILD
    )
endif()

#if(CMAKE_BUILD_TYPE STREQUAL Debug AND LINUX)
#    target_compile_options(parsers
#        -Wall -Wextra -Wuninitialized
#    )
#endif()

if(MACOS)
    set_target_properties(parsers PROPERTIES
        MACOSX_BUNDLE FALSE
    )
endif()

install(TARGETS parsers
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
