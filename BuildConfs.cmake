set(CMAKE_WARN_DEPRECATED OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

# By default the compiler used is clang/clang++
set(CMAKE_C_COMPILER /usr/bin/clang)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Importing Qt6 components
find_package(Qt6 REQUIRED COMPONENTS Core Gui Network PrintSupport Svg Widgets)

qt_standard_project_setup()
set(CMAKE_AUTORCC ON)

add_compile_definitions(QT_DISABLE_DEPRECATED_BEFORE=0x060400)

link_libraries(
    Qt::Core
    Qt::Gui
    Qt::Network
    Qt::PrintSupport
    Qt::Svg
    Qt::Widgets)


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
