cmake_minimum_required(VERSION 3.14)

# set the project name and version
project(canvas VERSION 1.0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(QT NAMES Qt6 Qt5 COMPONENTS
    Core
    Gui
    Widgets
    PrintSupport
    REQUIRED
)

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS
    Core
    Gui
    Widgets
    PrintSupport
    REQUIRED
)

set(HEADERS
    $ENV{LIBS_LIBCANVAS_SRC_PATH}/src/attributestoggleritem.h
    $ENV{LIBS_LIBCANVAS_SRC_PATH}/src/baseobjectview.h
    $ENV{LIBS_LIBCANVAS_SRC_PATH}/src/basetableview.h
    $ENV{LIBS_LIBCANVAS_SRC_PATH}/src/beziercurveitem.h
    $ENV{LIBS_LIBCANVAS_SRC_PATH}/src/graphicalview.h
    $ENV{LIBS_LIBCANVAS_SRC_PATH}/src/layeritem.h
    $ENV{LIBS_LIBCANVAS_SRC_PATH}/src/objectsscene.h
    $ENV{LIBS_LIBCANVAS_SRC_PATH}/src/relationshipview.h
    $ENV{LIBS_LIBCANVAS_SRC_PATH}/src/roundedrectitem.h
    $ENV{LIBS_LIBCANVAS_SRC_PATH}/src/schemaview.h
    $ENV{LIBS_LIBCANVAS_SRC_PATH}/src/styledtextboxview.h
    $ENV{LIBS_LIBCANVAS_SRC_PATH}/src/tableobjectview.h
    $ENV{LIBS_LIBCANVAS_SRC_PATH}/src/tabletitleview.h
    $ENV{LIBS_LIBCANVAS_SRC_PATH}/src/tableview.h
    $ENV{LIBS_LIBCANVAS_SRC_PATH}/src/textboxview.h
    $ENV{LIBS_LIBCANVAS_SRC_PATH}/src/textpolygonitem.h
)

set(SOURCES
    $ENV{LIBS_LIBCANVAS_SRC_PATH}/src/attributestoggleritem.cpp
    $ENV{LIBS_LIBCANVAS_SRC_PATH}/src/baseobjectview.cpp
    $ENV{LIBS_LIBCANVAS_SRC_PATH}/src/basetableview.cpp
    $ENV{LIBS_LIBCANVAS_SRC_PATH}/src/beziercurveitem.cpp
    $ENV{LIBS_LIBCANVAS_SRC_PATH}/src/graphicalview.cpp
    $ENV{LIBS_LIBCANVAS_SRC_PATH}/src/layeritem.cpp
    $ENV{LIBS_LIBCANVAS_SRC_PATH}/src/objectsscene.cpp
    $ENV{LIBS_LIBCANVAS_SRC_PATH}/src/relationshipview.cpp
    $ENV{LIBS_LIBCANVAS_SRC_PATH}/src/roundedrectitem.cpp
    $ENV{LIBS_LIBCANVAS_SRC_PATH}/src/schemaview.cpp
    $ENV{LIBS_LIBCANVAS_SRC_PATH}/src/styledtextboxview.cpp
    $ENV{LIBS_LIBCANVAS_SRC_PATH}/src/tableobjectview.cpp
    $ENV{LIBS_LIBCANVAS_SRC_PATH}/src/tabletitleview.cpp
    $ENV{LIBS_LIBCANVAS_SRC_PATH}/src/tableview.cpp
    $ENV{LIBS_LIBCANVAS_SRC_PATH}/src/textboxview.cpp
    $ENV{LIBS_LIBCANVAS_SRC_PATH}/src/textpolygonitem.cpp
)

add_library(canvas SHARED
    ${HEADERS}
    ${SOURCES}
)

#target_compile_definitions(canvas PRIVATE canvas_LIBRARY)

set(INCLUDE_DIRECTORIES
    ${Qt${QT_VERSION_MAJOR}Core_INCLUDE_DIRS}
    ${Qt${QT_VERSION_MAJOR}Gui_INCLUDE_DIRS}
    ${Qt${QT_VERSION_MAJOR}Widgets_INCLUDE_DIRS}
    ${Qt${QT_VERSION_MAJOR}PrintSupport_INCLUDE_DIRS}
    #
    $ENV{LIBS_LIBCANVAS_SRC_PATH}/src
    $ENV{LIBS_LIBCORE_SRC_PATH}/src
)

target_include_directories(canvas PUBLIC ${INCLUDE_DIRECTORIES})

target_link_libraries(canvas PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::PrintSupport
    #
    core
)

#Install

#install(TARGETS canvas
#        CONFIGURATIONS Debug
#        DESTINATION debug/bin)

#install(TARGETS canvas
#        CONFIGURATIONS Release
#        DESTINATION release/bin)

