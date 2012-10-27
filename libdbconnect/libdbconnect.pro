include(../pgmodeler.pro)

TEMPLATE = lib
TARGET = dbconnect
DESTDIR = ../build/lib
OBJECTS_DIR = obj
LIBS += $$DESTDIR/$$LIBUTIL

HEADERS += $$PWD/src/conexaobd.h \
           $$PWD/src/resultado.h

SOURCES += $$PWD/src/conexaobd.cpp \
           $$PWD/src/resultado.cpp
