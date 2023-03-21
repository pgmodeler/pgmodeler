cmake_minimum_required(VERSION 3.14)

# set the project name and version
project(PGMODELER VERSION 1.0)

set(ENV{LIBS_LIBCANVAS_SRC_PATH} ${CMAKE_SOURCE_DIR}/libs/libcanvas)
set(ENV{LIBS_LIBCONNECTOR_SRC_PATH} ${CMAKE_SOURCE_DIR}/libs/libconnector)
set(ENV{LIBS_LIBCORE_SRC_PATH} ${CMAKE_SOURCE_DIR}/libs/libcore)
set(ENV{LIBS_LIBGUI_SRC_PATH} ${CMAKE_SOURCE_DIR}/libs/libgui)
set(ENV{LIBS_LIBUTILS_SRC_PATH} ${CMAKE_SOURCE_DIR}/libs/libutils)
set(ENV{LIBS_LIBPARSERS_SRC_PATH} ${CMAKE_SOURCE_DIR}/libs/libparsers)

set(ENV{APPS_PGMODELER_SRC_PATH} ${CMAKE_SOURCE_DIR}/apps/pgmodeler)
set(ENV{APPS_PGMODELER_CH_SRC_PATH} ${CMAKE_SOURCE_DIR}/apps/pgmodeler-ch)
set(ENV{APPS_PGMODELER_CLI_SRC_PATH} ${CMAKE_SOURCE_DIR}/apps/pgmodeler-cli)
set(ENV{APPS_PGMODELER_SE_SRC_PATH} ${CMAKE_SOURCE_DIR}/apps/pgmodeler-se)

add_subdirectory(libs)
add_subdirectory(apps)
