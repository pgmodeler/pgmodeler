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
# NO_TESTS is set to true
CONFIG(debug, debug|release): {
	!isEqual(NO_TESTS, true):SUBDIRS += tests
}

# Deployment settings
isEqual(DEMO_VERSION, true): samples.files = assets/samples/demo.dbm
!isEqual(DEMO_VERSION, true): samples.files = assets/samples/*
samples.path = $$SAMPLESDIR

schemas.files = assets/schemas/*
schemas.path = $$SCHEMASDIR

lang.files = assets/lang/*
lang.path = $$LANGDIR

conf.files = assets/conf/*
conf.path = $$CONFDIR

doc.files = README.md CHANGELOG.md LICENSE RELEASENOTES.md
doc.path = $$DOCDIR

INSTALLS += samples schemas lang conf doc

CONFIG += qt
