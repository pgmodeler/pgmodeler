include(../pgmodeler.pro)

DESTDIR=$$LIBDESTDIR
TEMPLATE = lib
TARGET = parsers

LIBS += $$DESTDIR/$$LIBUTILS

HEADERS += src/schemaparser.h \
	   src/xmlparser.h

SOURCES += src/schemaparser.cpp \
	   src/xmlparser.cpp

