# This file contains the main settings to build subprojects
#
# Refactored version by: Lisandro Damián Nicanor Pérez Meyer <perezmeyer@gmail.com>
# Refactored code: https://github.com/perezmeyer/pgmodeler/tree/shared_libs
#
# Reviewed version by: Raphal Araújo e Silva <raphael@pgmodeler.com.br>
# Reviewed code: https://github.com/pgmodeler/pgmodeler
#
# NOTE: Reviewed code is not a direct merge from refactored version but based upon the
# refactored code, containing almost all changes done by the refactoring author.

include(pgmodeler.pri)

# Subprojects (libraries only)
SUBDIRS = libutils \
          libparsers \
          libpgmodeler \
          libpgconnector \
          libobjrenderer \
          libpgmodeler_ui

# Include the tests and plugins subprojects only on debug mode
CONFIG(debug, debug|release): SUBDIRS += tests plugins

# Including executables subprojects (libraries only)
SUBDIRS += crashhandler \
           main-cli \
           main

# Deployment settings
samples.files = samples/*
samples.path = $$SAMPLESDIR

schemas.files = schemas/*
schemas.path = $$SCHEMASDIR

lang.files = lang/*
lang.path = $$LANGDIR

conf.files = conf/*
conf.path = $$CONFDIR

doc.files = README.md CHANGELOG.md LICENSE RELEASENOTES.md
doc.path = $$DOCDIR

INSTALLS += samples schemas lang conf doc
