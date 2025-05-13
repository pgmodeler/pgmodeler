# By default the compiler used is clang/clang++
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

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
