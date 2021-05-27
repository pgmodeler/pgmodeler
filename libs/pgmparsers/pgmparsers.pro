include(../../pgmodeler.pri)

TEMPLATE = lib
TARGET = pgmparsers
windows: DESTDIR = $$PWD

HEADERS += src/schemaparser.h \
	   src/xmlparser.h \
	   src/attribsmap.h \
	    src/attributes.h

SOURCES += src/schemaparser.cpp \
	   src/xmlparser.cpp \
    src/attributes.cpp

unix|windows: LIBS += $$PGMUTILS_LIB $$XML_LIB

INCLUDEPATH += $$PGMUTILS_INC

DEPENDPATH += $$PGMUTILS_ROOT

# Deployment settings
target.path = $$PRIVATELIBDIR
INSTALLS = target
