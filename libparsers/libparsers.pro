contains(BUILDCONF, shared) {
 include(../pgmodeler.pro)
 TEMPLATE = lib
 TARGET = parsers
 DESTDIR = ../build/lib
 LIBS += $$DESTDIR/$$LIBUTIL
}

HEADERS += $$PWD/src/parseresquema.h \
           $$PWD/src/parserxml.h

SOURCES += $$PWD/src/parseresquema.cpp \
           $$PWD/src/parserxml.cpp
