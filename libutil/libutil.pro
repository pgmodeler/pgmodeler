include(../pgmodeler.pro)

TEMPLATE = lib
TARGET = util
DESTDIR = ../build/lib

HEADERS += $$PWD/src/excecao.h \
           $$PWD/src/atributosglobais.h \
           $$PWD/src/atributosparsers.h

SOURCES += $$PWD/src/excecao.cpp
