# This file contains the main settings to build subprojects
include(pgmodeler.pri)

# Subprojects (libraries only)
SUBDIRS = libs/$$LIBUTILS \
	  libs/$$LIBPARSERS \
	  libs/$$LIBCORE \
	  libs/$$LIBCONNECTOR \
	  libs/$$LIBCANVAS \
	  libs/$$LIBGUI

# Include the tests subprojects only on debug mode
CONFIG(debug, debug|release): SUBDIRS += tests

!exists($$PLUGINS_PRO_FILE) {
   !defined(PRIVATE_PLUGINS,var) {
     warning("The subproject $$PLUGINS_SRC_ROOT wasn't found! pgModeler will be build without plug-ins.")
     warning("If you want to compile pgModeler with plug-ins make sure to clone https://github.com/pgmodeler/plugins inside pgModeler's root folder and rerun qmake.")
   }

   defined(PRIVATE_PLUGINS,var) {
     error("The private plug-ins build was specified but the subproject $$PLUGINS_SRC_ROOT wasn't found! Aborting.")
   }
}

exists($$PLUGINS_PRO_FILE) {
   SUBDIRS += $$PLUGINS_FOLDER
}

# Including executables subprojects
SUBDIRS += apps/pgmodeler \
	   apps/pgmodeler-cli \
	   apps/pgmodeler-ch \
	   apps/pgmodeler-se

# Deployment settings
defined(DEMO_VERSION, var): samples.files = assets/samples/demo.dbm
!defined(DEMO_VERSION, var): samples.files = assets/samples/*
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
