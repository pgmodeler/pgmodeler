include(../pgmodeler.pro)

TEMPLATE = lib
TARGET = parsers

LIBS += $$DESTDIR/$$LIBUTILS \
	$$XML_LIB

HEADERS += src/schemaparser.h \
	   src/xmlparser.h \
	   src/attribsmap.h \
	   src/parsersattributes.h

SOURCES += src/schemaparser.cpp \
	   src/xmlparser.cpp

target.path = $$LIBDESTDIR
INSTALLS = target
