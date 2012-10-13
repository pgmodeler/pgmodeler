include(../pgmodeler.pro)

TEMPLATE = lib
TARGET = conexbd
DESTDIR = ../build/lib
OBJECTS_DIR = obj
LIBS += $$DESTDIR/$$LIBUTIL

HEADERS += src/conexaobd.h \
           src/resultado.h

SOURCES += src/conexaobd.cpp \
           src/resultado.cpp
