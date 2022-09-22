include(../../pgmodeler.pri)

TEMPLATE = lib
TARGET = parsers
windows: DESTDIR = $$PWD

HEADERS += src/schemaparser.h \
    src/csvdocument.h \
    src/csvparser.h \
	   src/xmlparser.h \
	   src/attribsmap.h \
	    src/attributes.h

SOURCES += src/schemaparser.cpp \
    src/csvdocument.cpp \
    src/csvparser.cpp \
	   src/xmlparser.cpp \
    src/attributes.cpp

unix|windows: LIBS += $$LIBUTILS_LIB $$XML_LIB

INCLUDEPATH += $$LIBUTILS_INC

DEPENDPATH += $$LIBUTILS_ROOT

# Deployment settings
target.path = $$PRIVATELIBDIR
INSTALLS = target
