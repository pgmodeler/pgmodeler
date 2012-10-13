include(../pgmodeler.pro)

TEMPLATE = lib
TARGET = objrenderer
DESTDIR = ../build/lib
LIBS += $$DESTDIR/$$LIBUTIL \
        $$DESTDIR/$$LIBPARSERS \
        $$DESTDIR/$$LIBPGMODELER

HEADERS +=  src/objetografico.h \
            src/ogcaixatexto.h \
            src/ogtabela.h \
            src/ogvisao.h \
            src/ogrelacionamento.h \
            src/ogtituloobjeto.h \
            src/ogsubitemobjeto.h \
            src/ogtabelabase.h \
            src/cenaobjetos.h

SOURCES +=  src/objetografico.cpp \
            src/ogcaixatexto.cpp \
            src/ogtabela.cpp \
            src/ogvisao.cpp \
            src/ogrelacionamento.cpp \
            src/ogtituloobjeto.cpp \
            src/ogsubitemobjeto.cpp \
            src/ogtabelabase.cpp \
            src/cenaobjetos.cpp
