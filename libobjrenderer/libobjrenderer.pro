include(../pgmodeler.pro)

TEMPLATE = lib
TARGET = objrenderer
DESTDIR = ../build/lib
LIBS += $$DESTDIR/$$LIBUTIL \
        $$DESTDIR/$$LIBPARSERS \
        $$DESTDIR/$$LIBPGMODELER

HEADERS +=  $$PWD/src/baseobjectview.h \
            $$PWD/src/textboxview.h \
            $$PWD/src/ogtabela.h \
            $$PWD/src/ogvisao.h \
            $$PWD/src/ogrelacionamento.h \
            $$PWD/src/ogtituloobjeto.h \
            $$PWD/src/ogsubitemobjeto.h \
            $$PWD/src/ogtabelabase.h \
            $$PWD/src/cenaobjetos.h

SOURCES +=  $$PWD/src/baseobjectview.cpp \
            $$PWD/src/textboxview.cpp \
            $$PWD/src/ogtabela.cpp \
            $$PWD/src/ogvisao.cpp \
            $$PWD/src/ogrelacionamento.cpp \
            $$PWD/src/ogtituloobjeto.cpp \
            $$PWD/src/ogsubitemobjeto.cpp \
            $$PWD/src/ogtabelabase.cpp \
            $$PWD/src/cenaobjetos.cpp
