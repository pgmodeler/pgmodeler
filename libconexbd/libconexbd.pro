#include(../pgmodeler.pro)

#CONFIG += qt warn_on uitools uic4
#QT = core gui qt3support
#TEMPLATE = lib
#TARGET = conexbd
#DESTDIR = ../build/lib
#DEPENDPATH = ". src obj"
#OBJECTS_DIR = obj
#LIBS += $$DESTDIR/$$LIBUTIL

HEADERS += $$PWD/src/conexaobd.h \
           $$PWD/src/resultado.h

SOURCES += $$PWD/src/conexaobd.cpp \
           $$PWD/src/resultado.cpp
