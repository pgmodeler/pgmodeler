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

if(BUILD_PRIV_PLUGINS)
    set(PRIV_GUI_SOURCES
        ${PRIV_PLUGINS_SRC}/pluginwidget.h
        ${PRIV_PLUGINS_SRC}/privateguiplugin.cpp
        ${PRIV_PLUGINS_SRC}/privateguiplugin.h
        ${PRIV_PLUGINS_SRC}/privpluginsglobal.h)

    set(PRIV_CLI_SOURCES
        ${PRIV_PLUGINS_SRC}/privatecliplugin.cpp
        ${PRIV_PLUGINS_SRC}/privatecliplugin.h
        ${PRIV_PLUGINS_SRC}/privpluginsglobal.h)
endif()

# This function adds the resource file (icons/images)
# in the list of plugin's sources to be compiled
# this will cause the RCC command to be called and
# generate the qrc_*.cpp which is linked with the
# plugin's lib
function(pgm_set_plugin_res TARGET)
    qt_add_resources(PLUGIN_RES "res/${TARGET}.qrc")
    target_sources(${TARGET} PRIVATE ${PLUGIN_RES})
endfunction()

# This function configures the deployment settings
# of the plugin library.
function(pgm_install_plugin_files)
    set(TARGET ${ARGV0})
    set(SRC_PATH ${PRIV_PLUGINS_ROOT}/${TARGET})

    foreach(file IN LISTS ARGN)
        list(APPEND FILES ${SRC_PATH}/${file})
    endforeach()

    install(FILES ${FILES}
        DESTINATION ${PGM_PLUGINSDIR}/${TARGET})
endfunction()

# This function configures the deployment settings
# of the plugin library.
function(pgm_install_plugin TARGET)
    install(TARGETS ${TARGET}
        LIBRARY DESTINATION ${PGM_PLUGINSDIR}/${TARGET}
        FRAMEWORK DESTINATION ${PGM_PLUGINSDIR}/${TARGET}
        RUNTIME DESTINATION ${PGM_PLUGINSDIR}/${TARGET})
endfunction()
