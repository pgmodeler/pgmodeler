include(../../pgmodeler.pri)

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

unix|windows: LIBS += $$LIBUTILS_LIB $$XML_LIB

INCLUDEPATH += $$LIBUTILS_INC

DEPENDPATH += $$LIBUTILS_ROOT

# Deployment settings
target.path = $$PRIVATELIBDIR
INSTALLS = target
