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
pgmodeler_data.files = samples schemas lang conf
pgmodeler_data.path = $$SHAREDIR

pgmodeler_doc.files = README.md CHANGELOG.md LICENSE RELEASENOTES.md
pgmodeler_doc.path = $$DOCDIR

INSTALLS += pgmodeler_data pgmodeler_doc
