# This file contains the main settings to build subprojects
include(pgmodeler.pri)

# Subprojects (libraries only)
SUBDIRS = libs/$$LIBUTILS \
	  libs/$$LIBPARSERS \
	  libs/$$LIBCORE \
	  libs/$$LIBCONNECTOR \
	  libs/$$LIBCANVAS \
	  libs/$$LIBGUI \
	  libs/$$LIBCLI

# Including executables subprojects
SUBDIRS += apps/pgmodeler \
	   apps/pgmodeler-cli \
	   apps/pgmodeler-ch \
	   apps/pgmodeler-se

!exists($$PLUGINS_PRO_FILE) {
	 !isEqual(PRIVATE_PLUGINS, true) {
		 warning("The subproject $$PLUGINS_SRC_ROOT wasn't found! pgModeler will be build without plug-ins.")
		 warning("If you want to compile pgModeler with plug-ins make sure to clone https://github.com/pgmodeler/plugins inside pgModeler's root folder and rerun qmake.")
	 }

	 isEqual(PRIVATE_PLUGINS, true) {
		 error("The private plug-ins build was specified but the subproject $$PLUGINS_SRC_ROOT wasn't found! Aborting.")
	 }
}

exists($$PLUGINS_PRO_FILE) {
	 SUBDIRS += $$PLUGINS_FOLDER
}

# Include the tests subprojects only on debug mode when
# BUILD_TESTS is set to true
CONFIG(debug, debug|release): {
	isEqual(BUILD_TESTS, true):SUBDIRS += tests
}

# Deployment settings
samples.files = assets/samples/*
samples.path = $$SAMPLESDIR

schemas.files = assets/schemas/*
schemas.path = $$SCHEMASDIR

lang.files = assets/lang/*
lang.path = $$LANGDIR

# Copies all template configs from assets/conf
TMPLCONFS = assets/conf
tmplconf.files = $$TMPLCONFS/*.conf \
$$TMPLCONFS/*.dbm \
$$TMPLCONFS/dtd \
$$TMPLCONFS/schemas \
$$TMPLCONFS/themes
tmplconf.path = $$CONFDIR

# Copies the icons in assest/conf
conficos.files = $$TMPLCONFS/*.png
conficos.path = $$CONFDIR

# Copies the highlight configs from dark theme to be the initial one
iniconf.files = $$TMPLCONFS/themes/dark/*-highlight.conf
iniconf.path = $$CONFDIR

# Creates the "defaults" configs dir containing the files from the initial template config dir
defconf.files = $$TMPLCONFS/themes/dark/*-highlight.conf \
$$TMPLCONFS/appearance.conf \
$$TMPLCONFS/connections.conf \
$$TMPLCONFS/diff-presets.conf \
$$TMPLCONFS/example.dbm \
$$TMPLCONFS/general.conf \
$$TMPLCONFS/example.dbm \
$$TMPLCONFS/general.conf \
$$TMPLCONFS/snippets.conf \
$$TMPLCONFS/relationships.conf \
$$TMPLCONFS/*-highlight.conf
defconf.path = $$CONFDIR/defaults

doc.files = README.md CHANGELOG.md LICENSE RELEASENOTES.md
doc.path = $$DOCDIR

INSTALLS += samples schemas lang doc tmplconf iniconf defconf conficos

# Including some private assets to deploy
isEqual(PRIVATE_PLUGINS, true) {
	privicons.files = $$PRIV_RES_FOLDER/icons/pgmodeler_logo.png
	privicons.path = $$CONFDIR

	INSTALLS += privicons
	INSTALLS -= conficos
}

CONFIG += qt
