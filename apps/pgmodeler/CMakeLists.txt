cmake_minimum_required(VERSION 3.14)

# set the project name and version
project(pgmodeler VERSION 1.0)

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
    REQUIRED
)

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS
    Core
    Gui
    Widgets
    REQUIRED
)


set(HEADERS
    $ENV{APPS_PGMODELER_SRC_PATH}/src/pgmodelerapp.h
)

set(SOURCES
    $ENV{APPS_PGMODELER_SRC_PATH}/src/main.cpp
    $ENV{APPS_PGMODELER_SRC_PATH}/src/pgmodelerapp.cpp
)

add_executable(pgmodeler
    ${HEADERS}
    ${SOURCES}
)

#target_compile_definitions(pgmodeler PRIVATE pgmodeler_LIBRARY)

set(INCLUDE_DIRECTORIES
    ${Qt${QT_VERSION_MAJOR}Core_INCLUDE_DIRS}
    ${Qt${QT_VERSION_MAJOR}Gui_INCLUDE_DIRS}
    ${Qt${QT_VERSION_MAJOR}Widgets_INCLUDE_DIRS}
    #
    $ENV{APPS_PGMODELER_SRC_PATH}/src
    #
    $ENV{LIBS_PGMODELER_LIBUTILS_SRC_PATH}/src
    $ENV{LIBS_PGMODELER_LIBGUI_SRC_PATH}/src
    #
    ${CMAKE_BINARY_DIR}/Libs/PgModeler/libgui/master
)

target_include_directories(pgmodeler PUBLIC ${INCLUDE_DIRECTORIES})

target_link_libraries(pgmodeler PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    #
    utils
    gui
)
#Install

install(TARGETS pgmodeler
        CONFIGURATIONS ${CMAKE_BUILD_TYPE}
        DESTINATION install/${CMAKE_BUILD_TYPE}/bin
)

