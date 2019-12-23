# libparsers.pro (reviewed version)
#
# Refactored by: Lisandro Damián Nicanor Pérez Meyer <perezmeyer@gmail.com>
# Refactored code: https://github.com/perezmeyer/pgmodeler/tree/shared_libs
# Reviewed by: Raphal Araújo e Silva <raphael@pgmodeler.com.br>
#
# NOTE: Reviewed code is not a direct merge from refactored version but based upon the
# refactored code, containing almost all changes done by the refactoring author.

include(../pgmodeler.pri)

TEMPLATE = lib
TARGET = parsers
windows: DESTDIR = $$PWD

HEADERS += src/schemaparser.h \
	   src/xmlparser.h \
	   src/attribsmap.h \
	    src/attributes.h

SOURCES += src/schemaparser.cpp \
	   src/xmlparser.cpp \
    src/attributes.cpp

unix|windows: LIBS += -L$$OUT_PWD/../libutils/ -lutils $$XML_LIB

INCLUDEPATH += $$PWD/../libutils/src
DEPENDPATH += $$PWD/../libutils

# Deployment settings
target.path = $$PRIVATELIBDIR
INSTALLS = target
