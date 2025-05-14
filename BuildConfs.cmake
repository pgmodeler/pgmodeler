set(CMAKE_WARN_DEPRECATED OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

# By default the compiler used is clang/clang++
set(CMAKE_C_COMPILER /usr/bin/clang)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Importing Qt6 components
# Forcing a version >= 6.4.0. If no suitable version is found the script aborts
find_package(Qt6 6.4.0 REQUIRED COMPONENTS Core Gui Network PrintSupport Svg Widgets)

qt_standard_project_setup()

# Disabling deprecated Qt code of version below 6.4
add_compile_definitions(QT_DISABLE_DEPRECATED_BEFORE=0x060400)

link_libraries(
    Qt::Core
    Qt::Gui
    Qt::Network
    Qt::PrintSupport
    Qt::Svg
    Qt::Widgets)

if(CMAKE_BUILD_TYPE STREQUAL Debug)
    add_compile_definitions(PGMODELER_DEBUG)
endif()

if(NOT DEFINED NO_CHECK_CURR_VER)
    add_compile_definitions(CHECK_CURR_VER)
endif()

if(SNAPSHOT_BUILD)
    add_compile_definitions(SNAPSHOT_BUILD)
endif()

if(DEMO_VERSION)
    add_compile_definitions(DEMO_VERSION)
endif()

if(NO_UPDATE_CHECK)
    add_compile_definitions(NO_UPDATE_CHECK)
endif()

if(PRIVATE_PLUGINS)
    add_compile_definitions(PRIVATE_PLUGINS)
endif()

if(APPIMAGE_BUILD AND LINUX)
    add_compile_definitions(APPIMAGE_BUILD)
endif()

# if(MACOS)
#     set_target_properties(canvas PROPERTIES
#         MACOSX_BUNDLE FALSE
#     )
# endif()
