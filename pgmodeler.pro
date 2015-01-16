include(pgmodeler.pri)

################################################################################
# Subprojects
################################################################################

SUBDIRS = libutils \
          libparsers \
          libpgmodeler \
          libpgconnector \
          libobjrenderer \
          libpgmodeler_ui \
          crashhandler \
          main-cli \
          main

#Include the tests subproject only on debug mode
CONFIG(debug, debug|release): SUBDIRS += tests plugins

################################################################################
# Deployment
################################################################################

pgmodeler_data.files = samples schemas lang conf
pgmodeler_data.path = $$SHAREDIR

pgmodeler_doc.files = README.md CHANGELOG.md LICENSE RELEASENOTES.md
pgmodeler_doc.path = $$DOCDIR

INSTALLS += pgmodeler_data pgmodeler_doc
