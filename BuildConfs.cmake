set(CMAKE_WARN_DEPRECATED OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

# By default the compiler used is clang/clang++
set(CMAKE_C_COMPILER /usr/bin/clang)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Importing Qt6 components
find_package(Qt6 REQUIRED COMPONENTS Core Gui Network PrintSupport Svg Test Widgets)

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
