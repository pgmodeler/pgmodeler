include(../pgmodeler.pro)

TEMPLATE = lib
TARGET = parsers
DESTDIR = ../build/lib
LIBS += $$DESTDIR/$$LIBUTIL

HEADERS += src/parseresquema.h \
           src/parserxml.h

SOURCES += src/parseresquema.cpp \
           src/parserxml.cpp
