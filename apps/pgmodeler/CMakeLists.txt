qt_add_executable(pgmodeler WIN32 MACOSX_BUNDLE
    src/main.cpp
    src/pgmodelerapp.cpp src/pgmodelerapp.h
)
target_include_directories(pgmodeler PRIVATE
    ${LIBCANVAS_INC}
    ${LIBCLI_INC}
    ${LIBCONNECTOR_INC}
    ${LIBCORE_INC}
    ${LIBGUI_INC}
    ${LIBPARSERS_INC}
    ${LIBUTILS_INC}
    ../../plugins/src
)

target_link_libraries(pgmodeler PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Network
    Qt::PrintSupport
    Qt::Svg
    Qt::Widgets
    canvas
    cli
    connector
    core
    gui
    parsers
    utils
)

install(TARGETS pgmodeler
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${BINDIR}
)

#qt_generate_deploy_app_script(
#    TARGET pgmodeler
#    FILENAME_VARIABLE deploy_script
#    NO_UNSUPPORTED_PLATFORM_ERROR
#)

#install(SCRIPT ${deploy_script})

# Copying the samples/*.dbm to the destination
install(DIRECTORY ../../assets/samples/
        DESTINATION ${SAMPLESDIR}
        FILES_MATCHING
        PATTERN "*.dbm")

# Copying the lang/* to the destination
#install(DIRECTORY ../../assets/lang/
#        DESTINATION ${LANGDIR}
#        FILES_MATCHING
#        PATTERN "*.*")

# Copying all folder under schemas/ to the destination
# Without recreating the schemas/ folder in the destination
file(GLOB sch_dirs LIST_DIRECTORIES true "../../assets/schemas/*")

foreach(sch_dir ${sch_dirs})
    # Extracts the name of the current schema directory
    get_filename_component(dirname ${sch_dir} NAME)

    if(IS_DIRECTORY ${sch_dir})
        install(DIRECTORY ${sch_dir}
                DESTINATION ${SCHEMASDIR}
                USE_SOURCE_PERMISSIONS)
    endif()
endforeach()

# Copying all template configs from assets/conf
install(DIRECTORY ../../assets/conf/
        DESTINATION ${CONFDIR}
        FILES_MATCHING
        PATTERN "*.conf"
        PATTERN "*.dbm"
        PATTERN "*.png")

install(DIRECTORY ../../assets/conf/schemas
        DESTINATION ${CONFDIR})

install(DIRECTORY ../../assets/conf/dtd
        DESTINATION ${CONFDIR})

# Copying  the highlight configs from dark theme to be the initial one
install(DIRECTORY ../../assets/conf/themes/dark/
        DESTINATION ${CONFDIR}
        FILES_MATCHING
        PATTERN "*-highlight.conf")

# Creating the "defaults" configs dir containing the files from the initial template config dir
install(DIRECTORY ../../assets/conf/themes/dark/
        DESTINATION ${CONFDIR}/defaults
        FILES_MATCHING
        PATTERN "*-highlight.conf")

install(DIRECTORY ../../assets/conf/
        DESTINATION ${CONFDIR}/defaults
        FILES_MATCHING
        PATTERN "*.conf"
        PATTERN "*.dbm"
        PATTERN "schemas" EXCLUDE
        PATTERN "themes" EXCLUDE
        PATTERN "dtd" EXCLUDE)

# Copying doc files
install(FILES
        ../../README.md
        ../../CHANGELOG.md
        ../../LICENSE
        ../../RELEASENOTES.md
        DESTINATION ${DOCDIR})

# # Including some private assets to deploy
# isEqual(PRIVATE_PLUGINS, true) {
# 	privicons.files = $$PRIV_RES_FOLDER/icons/pgmodeler_logo.png
# 	privicons.path = $$CONFDIR

# 	INSTALLS += privicons
# 	INSTALLS -= conficos
# }


