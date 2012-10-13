include(../pgmodeler.pro)

TEMPLATE = lib
TARGET = util
DESTDIR = ../build/lib

HEADERS += src/excecao.h \
           src/atributosglobais.h \
           src/atributosparsers.h

SOURCES += src/excecao.cpp
