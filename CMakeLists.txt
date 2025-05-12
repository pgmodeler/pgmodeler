cmake_minimum_required(VERSION 3.16)
project(pgmodeler VERSION 2.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui Network PrintSupport Svg Test Widgets)

qt_standard_project_setup()

add_subdirectory(libs/libutils)
add_subdirectory(libs/libparsers)
add_subdirectory(libs/libcore)
add_subdirectory(libs/libconnector)
add_subdirectory(libs/libcanvas)
add_subdirectory(libs/libgui)
add_subdirectory(libs/libcli)
add_subdirectory(apps/pgmodeler)
add_subdirectory(apps/pgmodeler-cli)
add_subdirectory(apps/pgmodeler-ch)
add_subdirectory(apps/pgmodeler-se)

if(BUILD_TESTS___equals___ON AND CMAKE_BUILD_TYPE STREQUAL Debug)
    add_subdirectory(tests)
endif()
