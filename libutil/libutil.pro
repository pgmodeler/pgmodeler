include(../pgmodeler.pro)

TEMPLATE = lib
TARGET = util
DESTDIR = ../build/lib

HEADERS += $$PWD/src/exception.h \
           $$PWD/src/atributosglobais.h \
           $$PWD/src/atributosparsers.h

SOURCES += $$PWD/src/exception.cpp
