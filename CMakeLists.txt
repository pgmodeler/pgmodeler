cmake_minimum_required(VERSION 3.16)
project(pgmodeler VERSION 2.0 LANGUAGES C CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
include(DefaultPaths)
include(BuildNumDate)
include(PrintSummary)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

# By default the compiler used is clang/clang++
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui Network PrintSupport Svg Test Widgets)

qt_standard_project_setup()
set(CMAKE_AUTORCC ON)

add_compile_definitions(
    QT_DISABLE_DEPRECATED_BEFORE=0x060400
)

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

if(BUILD_TESTS EQUAL ON AND CMAKE_BUILD_TYPE STREQUAL Debug)
    add_subdirectory(tests)
endif()


print_build_summary()



# if(UNIX OR windows)
#     target_link_libraries(canvas PUBLIC
#         # Remove: L(${CMAKE_CURRENT_SOURCE_DIR}/libs/libcore)
#         # Remove: L(${CMAKE_CURRENT_SOURCE_DIR}/libs/libparsers)
#         # Remove: L(${CMAKE_CURRENT_SOURCE_DIR}/libs/libutils)
#         core
#         parsers
#         utils
#     )
# endif()

# if(CONFIG___contains___debug)
#     target_compile_definitions(canvas PUBLIC
#         PGMODELER_DEBUG
#     )
# endif()

#if(NOT defined(NO_CHECK_CURR_VER,var))
#    target_compile_definitions(canvas PUBLIC
#        CHECK_CURR_VER
#    )
#endif()

# if(SNAPSHOT_BUILD___equals___ON)
#     target_compile_definitions(canvas PUBLIC
#         SNAPSHOT_BUILD
#     )
# endif()

# if(DEMO_VERSION___equals___ON)
#     target_compile_definitions(canvas PUBLIC
#         DEMO_VERSION
#     )
# endif()

# if(NO_UPDATE_CHECK___equals___ON)
#     target_compile_definitions(canvas PUBLIC
#         NO_UPDATE_CHECK
#     )
# endif()

# if(PRIVATE_PLUGINS___equals___ON)
#     target_compile_definitions(canvas PUBLIC
#         PRIVATE_PLUGINS_SYMBOLS
#     )
# endif()

# if(APPIMAGE_BUILD___equals___ON AND LINUX)
#     target_compile_definitions(canvas PUBLIC
#         APPIMAGE_BUILD
#     )
# endif()

#if(CMAKE_BUILD_TYPE STREQUAL Debug AND LINUX)
#    target_compile_options(canvas
#        -Wall -Wextra -Wuninitialized
#    )
#endif()

# if(MACOS)
#     set_target_properties(canvas PROPERTIES
#         MACOSX_BUNDLE FALSE
#     )
# endif()
