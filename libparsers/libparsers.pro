include(../pgmodeler.pro)

TEMPLATE = lib
TARGET = parsers
DESTDIR = ../build/lib
LIBS += $$DESTDIR/$$LIBUTIL

HEADERS += $$PWD/src/schemaparser.h \
           $$PWD/src/xmlparser.h

SOURCES += $$PWD/src/schemaparser.cpp \
           $$PWD/src/xmlparser.cpp
