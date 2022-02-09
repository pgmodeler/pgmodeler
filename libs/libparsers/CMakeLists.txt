cmake_minimum_required(VERSION 3.14)

# set the project name and version
project(parsers VERSION 1.0)

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

find_package(LibXml2 REQUIRED)

set(HEADERS
    $ENV{LIBS_LIBPARSERS_SRC_PATH}/src/attribsmap.h
    $ENV{LIBS_LIBPARSERS_SRC_PATH}/src/attributes.h
    $ENV{LIBS_LIBPARSERS_SRC_PATH}/src/schemaparser.h
    $ENV{LIBS_LIBPARSERS_SRC_PATH}/src/xmlparser.h
)

set(SOURCES
    $ENV{LIBS_LIBPARSERS_SRC_PATH}/src/attributes.cpp
    $ENV{LIBS_LIBPARSERS_SRC_PATH}/src/schemaparser.cpp
    $ENV{LIBS_LIBPARSERS_SRC_PATH}/src/xmlparser.cpp
)

add_library(parsers SHARED ${HEADERS} ${SOURCES})

target_compile_definitions(parsers PRIVATE

)

set(INCLUDE_DIRECTORIES
    $ENV{LIBS_LIBUTILS_SRC_PATH}/src
    $ENV{LIBS_LIBPARSERS_SRC_PATH}/src
    #
    ${Qt${QT_VERSION_MAJOR}Core_INCLUDE_DIRS}
    #
    ${LIBXML2_INCLUDE_DIR}
)


target_include_directories(parsers PUBLIC ${INCLUDE_DIRECTORIES})

#target_link_directories(parsers PUBLIC ${CMAKE_BINARY_DIR}/Libs/PgModeler/libutils/master)

target_link_libraries(parsers PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    #
    ${LIBXML2_LIBRARIES}
    #
    utils
)

#Install

#install(TARGETS parsers
#        CONFIGURATIONS Debug
#        DESTINATION debug/bin)

#install(TARGETS parsers
#        CONFIGURATIONS Release
#        DESTINATION release/bin)

