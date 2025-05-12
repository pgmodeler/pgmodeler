set(utils_src
    src/application.cpp src/application.h
    src/attribsmap.h
    src/customuistyle.cpp src/customuistyle.h
    src/doublenan.h
    src/enumtype.h
    src/exception.cpp src/exception.h
    src/globalattributes.cpp src/globalattributes.h
    src/pgmodelerplugin.cpp src/pgmodelerplugin.h
    src/pgsqlversions.cpp src/pgsqlversions.h
    src/utilsglobal.h
    src/utilsns.cpp src/utilsns.h
)

qt_add_library(utils SHARED
    ${utils_src}
)

target_include_directories(utils PUBLIC
    ../../plugins/src
)

target_compile_definitions(utils PUBLIC
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
    UTILS_SYMBOLS
)

target_link_libraries(utils PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Network
    Qt::PrintSupport
    Qt::Svg
    Qt::Widgets
)

if(CONFIG___contains___debug)
    target_compile_definitions(utils PUBLIC
        PGMODELER_DEBUG
    )
endif()

#if(NOT defined(NO_CHECK_CURR_VER,var))
#    target_compile_definitions(utils PUBLIC
#        CHECK_CURR_VER
#    )
#endif()

if(SNAPSHOT_BUILD___equals___ON)
    target_compile_definitions(utils PUBLIC
        SNAPSHOT_BUILD
    )
endif()

if(DEMO_VERSION___equals___ON)
    target_compile_definitions(utils PUBLIC
        DEMO_VERSION
    )
endif()

if(NO_UPDATE_CHECK___equals___ON)
    target_compile_definitions(utils PUBLIC
        NO_UPDATE_CHECK
    )
endif()

if(PRIVATE_PLUGINS___equals___ON)
    target_compile_definitions(utils PUBLIC
        PRIVATE_PLUGINS_SYMBOLS
    )
endif()

if(APPIMAGE_BUILD___equals___ON AND LINUX)
    target_compile_definitions(utils PUBLIC
        APPIMAGE_BUILD
    )
endif()

#if(CMAKE_BUILD_TYPE STREQUAL Debug AND LINUX)
#    target_compile_options(utils
#        -Wall -Wextra -Wuninitialized
#    )
#endif()

if(MACOS)
    set_target_properties(utils PROPERTIES
        MACOSX_BUNDLE FALSE
    )
endif()

install(TARGETS utils
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
