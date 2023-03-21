cmake_minimum_required(VERSION 3.14)

# set the project name and version
project(pgmodeler-cli VERSION 1.0)

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


set(HEADERS
    $ENV{APPS_PGMODELER_CLI_SRC_PATH}/src/pgmodelercliapp.h
)

set(SOURCES
    $ENV{APPS_PGMODELER_CLI_SRC_PATH}/src/main.cpp
    $ENV{APPS_PGMODELER_CLI_SRC_PATH}/src/pgmodelercliapp.cpp
)

add_executable(pgmodeler-cli
    ${HEADERS}
    ${SOURCES}
)

#target_compile_definitions(pgmodeler-cli PRIVATE pgmodeler_LIBRARY)

set(INCLUDE_DIRECTORIES
    ${Qt${QT_VERSION_MAJOR}Core_INCLUDE_DIRS}
    #
    $ENV{APPS_PGMODELER_CLI_SRC_PATH}/src
    #
    $ENV{LIBS_PGMODELER_LIBUTILS_SRC_PATH}/src
    $ENV{LIBS_PGMODELER_LIBGUI_SRC_PATH}/src
    #
    ${CMAKE_BINARY_DIR}/Libs/PgModeler/libgui/master
)

target_include_directories(pgmodeler-cli PUBLIC ${INCLUDE_DIRECTORIES})

target_link_libraries(pgmodeler-cli PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    #
    utils
    gui
)
#Install

install(TARGETS pgmodeler-cli
        CONFIGURATIONS ${CMAKE_BUILD_TYPE}
        DESTINATION install/${CMAKE_BUILD_TYPE}/bin
)


