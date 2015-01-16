include(../pgmodeler.pri)

TEMPLATE = lib
TARGET = parsers

HEADERS += src/schemaparser.h \
	   src/xmlparser.h \
	   src/attribsmap.h \
	   src/parsersattributes.h

SOURCES += src/schemaparser.cpp \
	   src/xmlparser.cpp

unix|win32: LIBS += -L$$OUT_PWD/../libutils/ -lutils

INCLUDEPATH += $$PWD/../libutils/src
DEPENDPATH += $$PWD/../libutils

# Installation
target.path = $$PRIVATELIBDIR
INSTALLS = target
