include(../pgmodeler.pri)

TEMPLATE = lib
TARGET = parsers
windows: DESTDIR = $$PWD

HEADERS += src/schemaparser.h \
	   src/xmlparser.h \
	   src/attribsmap.h \
	   src/parsersattributes.h

SOURCES += src/schemaparser.cpp \
	   src/xmlparser.cpp

unix|windows: LIBS += -L$$OUT_PWD/../libutils/ -lutils $$XML_LIB

INCLUDEPATH += $$PWD/../libutils/src
DEPENDPATH += $$PWD/../libutils

# Deployment settings
target.path = $$PRIVATELIBDIR
INSTALLS = target
