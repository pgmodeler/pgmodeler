include(../pgmodeler.pro)

CONFIG += qt warn_on uitools uic4
QT = core gui qt3support
TEMPLATE = lib
TARGET = objrenderer
DESTDIR = ../build/lib
DEPENDPATH = ". src obj"
OBJECTS_DIR = obj

LIBS += $$DESTDIR/$$LIBUTIL \
        $$DESTDIR/$$LIBPARSERS \
        $$DESTDIR/$$LIBPGMODELER

HEADERS +=  $$PWD/src/objetografico.h \
            $$PWD/src/ogcaixatexto.h \
            $$PWD/src/ogtabela.h \
            $$PWD/src/ogvisao.h \
            $$PWD/src/ogrelacionamento.h \
            $$PWD/src/ogtituloobjeto.h \
            $$PWD/src/ogsubitemobjeto.h \
            $$PWD/src/ogtabelabase.h \
            $$PWD/src/cenaobjetos.h

SOURCES +=  $$PWD/src/objetografico.cpp \
            $$PWD/src/ogcaixatexto.cpp \
            $$PWD/src/ogtabela.cpp \
            $$PWD/src/ogvisao.cpp \
            $$PWD/src/ogrelacionamento.cpp \
            $$PWD/src/ogtituloobjeto.cpp \
            $$PWD/src/ogsubitemobjeto.cpp \
            $$PWD/src/ogtabelabase.cpp \
            $$PWD/src/cenaobjetos.cpp
