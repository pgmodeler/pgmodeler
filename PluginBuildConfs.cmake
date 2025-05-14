# Importing Qt6 components
# Forcing a version >= 6.4.0. If no suitable version is found the script aborts
find_package(Qt6 6.4.0 REQUIRED COMPONENTS UiTools)

# By default, the AUTOUIC option makes the uic look for the .ui files
# under the same folder as the sources. In this case, we force the uic
# command to look for .ui files under ui/*
set(CMAKE_AUTOUIC_SEARCH_PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}
    ui)

include_directories(
    ${LIBCANVAS_INC}
    ${LIBCONNECTOR_INC}
    ${LIBCORE_INC}
    ${LIBCLI_INC}
    ${LIBPARSERS_INC}
    ${LIBUTILS_INC}
    ${LIBGUI_INC}
    ${PRIV_PLUGINS_SRC})

link_libraries(
    Qt::Core
    Qt::Gui
    Qt::UiTools
    canvas
    cli
    connector
    core
    gui
    parsers
    utils)

set(PRIV_GUI_SOURCES
    ${PRIV_PLUGINS_SRC}/pluginwidget.h
    ${PRIV_PLUGINS_SRC}/privateguiplugin.cpp
    ${PRIV_PLUGINS_SRC}/privateguiplugin.h
    ${PRIV_PLUGINS_SRC}/privpluginsglobal.h)

set(PRIV_CLI_SOURCES
    ${PRIV_PLUGINS_SRC}/privatecliplugin.cpp
    ${PRIV_PLUGINS_SRC}/privatecliplugin.h
    ${PRIV_PLUGINS_SRC}/privpluginsglobal.h)
