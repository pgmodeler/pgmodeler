qt_add_executable(csvparsertest WIN32 MACOSX_BUNDLE
    ../../src/pgmodelerunittest.h
    csvparsertest.cpp
)
target_include_directories(csvparsertest PRIVATE
    (${CMAKE_CURRENT_SOURCE_DIR}/../../libs/libcanvas)/src
    (${CMAKE_CURRENT_SOURCE_DIR}/../../libs/libconnector)/src
    (${CMAKE_CURRENT_SOURCE_DIR}/../../libs/libcore)/src
    (${CMAKE_CURRENT_SOURCE_DIR}/../../libs/libgui)/src
    (${CMAKE_CURRENT_SOURCE_DIR}/../../libs/libparsers)/src
    (${CMAKE_CURRENT_SOURCE_DIR}/../../libs/libutils)/src
    ../../../plugins/src
    ../../src
)

target_compile_definitions(csvparsertest PRIVATE
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

target_link_libraries(csvparsertest PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Network
    Qt::PrintSupport
    Qt::Svg
    Qt::Test
    Qt::Widgets
)

if(UNIX OR windows)
    target_link_libraries(csvparsertest PRIVATE
        # Remove: L(${CMAKE_CURRENT_SOURCE_DIR}/../../libs/libcanvas)
        # Remove: L(${CMAKE_CURRENT_SOURCE_DIR}/../../libs/libconnector)
        # Remove: L(${CMAKE_CURRENT_SOURCE_DIR}/../../libs/libcore)
        # Remove: L(${CMAKE_CURRENT_SOURCE_DIR}/../../libs/libgui)
        # Remove: L(${CMAKE_CURRENT_SOURCE_DIR}/../../libs/libparsers)
        # Remove: L(${CMAKE_CURRENT_SOURCE_DIR}/../../libs/libutils)
        canvas
        connector
        core
        gui
        parsers
        utils
    )
endif()

if(CONFIG___contains___debug)
    target_compile_definitions(csvparsertest PRIVATE
        PGMODELER_DEBUG
    )
endif()

if(NOT defined(NO_CHECK_CURR_VER,var))
    target_compile_definitions(csvparsertest PRIVATE
        CHECK_CURR_VER
    )
endif()

if(SNAPSHOT_BUILD___equals___ON)
    target_compile_definitions(csvparsertest PRIVATE
        SNAPSHOT_BUILD
    )
endif()

if(DEMO_VERSION___equals___ON)
    target_compile_definitions(csvparsertest PRIVATE
        DEMO_VERSION
    )
endif()

if(NO_UPDATE_CHECK___equals___ON)
    target_compile_definitions(csvparsertest PRIVATE
        NO_UPDATE_CHECK
    )
endif()

if(PRIVATE_PLUGINS___equals___ON)
    target_compile_definitions(csvparsertest PRIVATE
        PRIVATE_PLUGINS_SYMBOLS
    )
endif()

if(APPIMAGE_BUILD___equals___ON AND LINUX)
    target_compile_definitions(csvparsertest PRIVATE
        APPIMAGE_BUILD
    )
endif()

if(CMAKE_BUILD_TYPE STREQUAL Debug AND LINUX)
    target_compile_options(csvparsertest
        -Wall -Wextra -Wuninitialized
    )
endif()

if(MACOS)
    set_target_properties(csvparsertest PROPERTIES
        MACOSX_BUNDLE FALSE
    )
endif()

install(TARGETS csvparsertest
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(
    TARGET csvparsertest
    FILENAME_VARIABLE deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
