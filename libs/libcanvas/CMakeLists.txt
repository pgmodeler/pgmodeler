qt_add_library(canvas SHARED
    src/attributestoggleritem.cpp src/attributestoggleritem.h
    src/baseobjectview.cpp src/baseobjectview.h
    src/basetableview.cpp src/basetableview.h
    src/beziercurveitem.cpp src/beziercurveitem.h
    src/canvasglobal.h
    src/graphicalview.cpp src/graphicalview.h
    src/layeritem.cpp src/layeritem.h
    src/objectsscene.cpp src/objectsscene.h
    src/relationshipview.cpp src/relationshipview.h
    src/roundedrectitem.cpp src/roundedrectitem.h
    src/schemaview.cpp src/schemaview.h
    src/styledtextboxview.cpp src/styledtextboxview.h
    src/tableobjectview.cpp src/tableobjectview.h
    src/tabletitleview.cpp src/tabletitleview.h
    src/tableview.cpp src/tableview.h
    src/textboxview.cpp src/textboxview.h
    src/textpolygonitem.cpp src/textpolygonitem.h
)
target_include_directories(canvas PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/libcore/src
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/libparsers/src
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/libutils/src
    ../../plugins/src
)

target_compile_definitions(canvas PUBLIC
    BINDIR=\\\"\\\"
    BUILDDATE=\\\"\\\"
    BUILDNUM=\\\"\\\"
    CANVAS_SYMBOLS
    CONFDIR=\\\"\\\"
    DOCDIR=\\\"\\\"
    LANGDIR=\\\"\\\"
    PLUGINSDIR=\\\"\\\"
    PRIVATEBINDIR=\\\"\\\"
    QT_DISABLE_DEPRECATED_BEFORE=0x060400
    SAMPLESDIR=\\\"\\\"
    SCHEMASDIR=\\\"\\\"
)

target_link_libraries(canvas PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Network
    Qt::PrintSupport
    Qt::Svg
    Qt::Widgets
)

if(UNIX OR windows)
    target_link_libraries(canvas PUBLIC
        # Remove: L(${CMAKE_CURRENT_SOURCE_DIR}/libs/libcore)
        # Remove: L(${CMAKE_CURRENT_SOURCE_DIR}/libs/libparsers)
        # Remove: L(${CMAKE_CURRENT_SOURCE_DIR}/libs/libutils)
        core
        parsers
        utils
    )
endif()

if(CONFIG___contains___debug)
    target_compile_definitions(canvas PUBLIC
        PGMODELER_DEBUG
    )
endif()

#if(NOT defined(NO_CHECK_CURR_VER,var))
#    target_compile_definitions(canvas PUBLIC
#        CHECK_CURR_VER
#    )
#endif()

if(SNAPSHOT_BUILD___equals___ON)
    target_compile_definitions(canvas PUBLIC
        SNAPSHOT_BUILD
    )
endif()

if(DEMO_VERSION___equals___ON)
    target_compile_definitions(canvas PUBLIC
        DEMO_VERSION
    )
endif()

if(NO_UPDATE_CHECK___equals___ON)
    target_compile_definitions(canvas PUBLIC
        NO_UPDATE_CHECK
    )
endif()

if(PRIVATE_PLUGINS___equals___ON)
    target_compile_definitions(canvas PUBLIC
        PRIVATE_PLUGINS_SYMBOLS
    )
endif()

if(APPIMAGE_BUILD___equals___ON AND LINUX)
    target_compile_definitions(canvas PUBLIC
        APPIMAGE_BUILD
    )
endif()

#if(CMAKE_BUILD_TYPE STREQUAL Debug AND LINUX)
#    target_compile_options(canvas
#        -Wall -Wextra -Wuninitialized
#    )
#endif()

if(MACOS)
    set_target_properties(canvas PROPERTIES
        MACOSX_BUNDLE FALSE
    )
endif()

install(TARGETS canvas
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
