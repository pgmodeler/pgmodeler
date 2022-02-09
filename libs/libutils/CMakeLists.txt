cmake_minimum_required(VERSION 3.14)

# set the project name and version
project(utils VERSION 1.0)

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
    $ENV{LIBS_LIBUTILS_SRC_PATH}/src/application.h
    $ENV{LIBS_LIBUTILS_SRC_PATH}/src/doublenan.h
    $ENV{LIBS_LIBUTILS_SRC_PATH}/src/exception.h
    $ENV{LIBS_LIBUTILS_SRC_PATH}/src/globalattributes.h
    $ENV{LIBS_LIBUTILS_SRC_PATH}/src/pgsqlversions.h
    $ENV{LIBS_LIBUTILS_SRC_PATH}/src/utilsns.h

    $ENV{LIBS_LIBUTILS_SRC_PATH}/src/qtcompat/qfontmetricscompat.h
    $ENV{LIBS_LIBUTILS_SRC_PATH}/src/qtcompat/qlabelcompat.h
    $ENV{LIBS_LIBUTILS_SRC_PATH}/src/qtcompat/qlinefcompat.h
    $ENV{LIBS_LIBUTILS_SRC_PATH}/src/qtcompat/qplaintexteditcompat.h
    $ENV{LIBS_LIBUTILS_SRC_PATH}/src/qtcompat/qtextstreamcompat.h
    $ENV{LIBS_LIBUTILS_SRC_PATH}/src/qtcompat/splitbehaviorcompat.h
)

set(SOURCES
    $ENV{LIBS_LIBUTILS_SRC_PATH}/src/application.cpp
    $ENV{LIBS_LIBUTILS_SRC_PATH}/src/exception.cpp
    $ENV{LIBS_LIBUTILS_SRC_PATH}/src/globalattributes.cpp
    $ENV{LIBS_LIBUTILS_SRC_PATH}/src/pgsqlversions.cpp
    $ENV{LIBS_LIBUTILS_SRC_PATH}/src/utilsns.cpp

    $ENV{LIBS_LIBUTILS_SRC_PATH}/src/qtcompat/qfontmetricscompat.cpp
    $ENV{LIBS_LIBUTILS_SRC_PATH}/src/qtcompat/qlabelcompat.cpp
    $ENV{LIBS_LIBUTILS_SRC_PATH}/src/qtcompat/qlinefcompat.cpp
    $ENV{LIBS_LIBUTILS_SRC_PATH}/src/qtcompat/qplaintexteditcompat.cpp
    $ENV{LIBS_LIBUTILS_SRC_PATH}/src/qtcompat/qtextstreamcompat.cpp
)

add_library(utils SHARED ${HEADERS} ${SOURCES})

target_compile_definitions(utils PRIVATE
    BUILDNUM
    BUILDDATE
    SCHEMASDIR=\"\"
    LANGDIR=\"\"
    SAMPLESDIR=\"\"
    CONFDIR=\"\"
    PLUGINSDIR=\"\"
    PRIVATEBINDIR=\"\"
    BINDIR=\"\"
)

set(INCLUDE_DIRECTORIES
    $ENV{LIBS_LIBUTILS_SRC_PATH}/src
    $ENV{LIBS_LIBUTILS_SRC_PATH}/src/qtcompat
    #
    Qt${QT_VERSION_MAJOR}Core_INCLUDE_DIRS
    Qt${QT_VERSION_MAJOR}Gui_INCLUDE_DIRS
    Qt${QT_VERSION_MAJOR}Widgets_INCLUDE_DIRS
)

target_include_directories(utils PUBLIC ${INCLUDE_DIRECTORIES})

target_link_libraries(utils PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
)


#Install

#install(TARGETS utils
#        CONFIGURATIONS Debug
#        DESTINATION debug/bin)

#install(TARGETS utils
#        CONFIGURATIONS Release
#        DESTINATION release/bin)

