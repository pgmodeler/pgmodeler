cmake_minimum_required(VERSION 3.14)

# set the project name and version
project(libconnector VERSION 1.0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(QT NAMES Qt6 Qt5 COMPONENTS
    Core
    REQUIRED
)

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS
    Core
    REQUIRED
)

find_package(PostgreSQL REQUIRED)

set(HEADERS
    $ENV{LIBS_LIBCONNECTOR_SRC_PATH}/src/catalog.h
    $ENV{LIBS_LIBCONNECTOR_SRC_PATH}/src/connection.h
    $ENV{LIBS_LIBCONNECTOR_SRC_PATH}/src/resultset.h
)

set(SOURCES
    $ENV{LIBS_LIBCONNECTOR_SRC_PATH}/src/catalog.cpp
    $ENV{LIBS_LIBCONNECTOR_SRC_PATH}/src/connection.cpp
    $ENV{LIBS_LIBCONNECTOR_SRC_PATH}/src/resultset.cpp
)

add_library(connector SHARED
    ${HEADERS}
    ${SOURCES}
)

#target_compile_definitions(connector PRIVATE connector_LIBRARY)

set(INCLUDE_DIRECTORIES
    $ENV{LIBS_LIBCONNECTOR_SRC_PATH}/src
    $ENV{LIBS_LIBUTILS_SRC_PATH}/src
    $ENV{LIBS_LIBPARSERS_SRC_PATH}/src
    $ENV{LIBS_LIBCORE_SRC_PATH}/src
    #
    ${Qt${QT_VERSION_MAJOR}Core_INCLUDE_DIRS}
)

target_include_directories(connector PUBLIC ${INCLUDE_DIRECTORIES})

target_link_libraries(connector PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    #
    PostgreSQL::PostgreSQL
    #
    utils
    parsers
    core
)

#Install

#install(TARGETS connector
#        CONFIGURATIONS Debug
#        DESTINATION debug/bin)

#install(TARGETS connector
#        CONFIGURATIONS Release
#        DESTINATION release/bin)

