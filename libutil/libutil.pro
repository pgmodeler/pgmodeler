include(../pgmodeler.pro)

CONFIG += qt warn_on uitools uic4
QT = core gui qt3support
TEMPLATE = lib
TARGET = util
DESTDIR = ../build/lib
DEPENDPATH = ". res src ui moc obj"
OBJECTS_DIR = obj

HEADERS += $$PWD/src/excecao.h \
           $$PWD/src/atributosglobais.h \
           $$PWD/src/atributosparsers.h

SOURCES += $$PWD/src/excecao.cpp
