qt_add_executable(pgmodeler-se WIN32 MACOSX_BUNDLE
    src/aboutsewidget.cpp src/aboutsewidget.h
    src/main.cpp
    src/schemaeditorform.cpp src/schemaeditorform.h
    src/sourceeditorwidget.cpp src/sourceeditorwidget.h
    ui/aboutsewidget.ui
    ui/schemaeditorform.ui
    ui/sourceeditorwidget.ui
)
target_include_directories(pgmodeler-se PRIVATE
    (${CMAKE_CURRENT_SOURCE_DIR}/../libs/libcanvas)/src
    (${CMAKE_CURRENT_SOURCE_DIR}/../libs/libcli)/src
    (${CMAKE_CURRENT_SOURCE_DIR}/../libs/libconnector)/src
    (${CMAKE_CURRENT_SOURCE_DIR}/../libs/libcore)/src
    (${CMAKE_CURRENT_SOURCE_DIR}/../libs/libgui)/src
    (${CMAKE_CURRENT_SOURCE_DIR}/../libs/libparsers)/src
    (${CMAKE_CURRENT_SOURCE_DIR}/../libs/libutils)/src
    ../../plugins/src
    ../pgmodeler/src
)

target_compile_definitions(pgmodeler-se PRIVATE
    BINDIR=\\\"\\\"
    BUILDDATE=\\\"\\\"
    BUILDNUM=\\\"\\\"
    CONFDIR=\\\"\\\"
    DOCDIR=\\\"\\\"
    LANGDIR=\\\"\\\"
    PLUGINSDIR=\\\"\\\"
    PRIVATEBINDIR=\\\"\\\"
    QT_DISABLE_DEPRECATED_BEFORE=0x060400
    SAMPLESDIR=\\\"\\\"
    SCHEMASDIR=\\\"\\\"
)

target_link_libraries(pgmodeler-se PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Network
    Qt::PrintSupport
    Qt::Svg
    Qt::Widgets
)

if(PRIVATE_PLUGINS___equals___ON)
    target_compile_definitions(pgmodeler-se PRIVATE
        PRIVATE_PLUGINS_SYMBOLS
    )
endif()

if(UNIX OR windows)
    target_link_libraries(pgmodeler-se PRIVATE
        # Remove: L(${CMAKE_CURRENT_SOURCE_DIR}/../libs/libcanvas)
        # Remove: L(${CMAKE_CURRENT_SOURCE_DIR}/../libs/libcli)
        # Remove: L(${CMAKE_CURRENT_SOURCE_DIR}/../libs/libconnector)
        # Remove: L(${CMAKE_CURRENT_SOURCE_DIR}/../libs/libcore)
        # Remove: L(${CMAKE_CURRENT_SOURCE_DIR}/../libs/libgui)
        # Remove: L(${CMAKE_CURRENT_SOURCE_DIR}/../libs/libparsers)
        # Remove: L(${CMAKE_CURRENT_SOURCE_DIR}/../libs/libutils)
        canvas
        cli
        connector
        core
        gui
        parsers
        utils
    )
endif()

if(CONFIG___contains___debug)
    target_compile_definitions(pgmodeler-se PRIVATE
        PGMODELER_DEBUG
    )
endif()

#if(NOT defined(NO_CHECK_CURR_VER,var))
#    target_compile_definitions(pgmodeler-se PRIVATE
#        CHECK_CURR_VER
#    )
#endif()

if(SNAPSHOT_BUILD___equals___ON)
    target_compile_definitions(pgmodeler-se PRIVATE
        SNAPSHOT_BUILD
    )
endif()

if(DEMO_VERSION___equals___ON)
    target_compile_definitions(pgmodeler-se PRIVATE
        DEMO_VERSION
    )
endif()

if(NO_UPDATE_CHECK___equals___ON)
    target_compile_definitions(pgmodeler-se PRIVATE
        NO_UPDATE_CHECK
    )
endif()

if(APPIMAGE_BUILD___equals___ON AND LINUX)
    target_compile_definitions(pgmodeler-se PRIVATE
        APPIMAGE_BUILD
    )
endif()

#if(CMAKE_BUILD_TYPE STREQUAL Debug AND LINUX)
#    target_compile_options(pgmodeler-se
#        -Wall -Wextra -Wuninitialized
#    )
#endif()

if(MACOS)
    set_target_properties(pgmodeler-se PROPERTIES
        MACOSX_BUNDLE FALSE
    )
endif()

install(TARGETS pgmodeler-se
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(
    TARGET pgmodeler-se
    FILENAME_VARIABLE deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
