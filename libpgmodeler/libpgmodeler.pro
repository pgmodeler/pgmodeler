include(../pgmodeler.pro)

TEMPLATE = lib
TARGET = pgmodeler
DESTDIR = ../build/lib

LIBS += $$DESTDIR/$$LIBUTIL \
        $$DESTDIR/$$LIBPARSERS

HEADERS += $$PWD/src/textbox.h \
           $$PWD/src/cast.h \
           $$PWD/src/funcao.h \
           $$PWD/src/linguagem.h \
           $$PWD/src/operador.h \
           $$PWD/src/relacionamentobase.h \
           $$PWD/src/basetable.h \
           $$PWD/src/column.h \
           $$PWD/src/domain.h \
           $$PWD/src/funcaoagregacao.h \
           $$PWD/src/modelobd.h \
           $$PWD/src/papel.h \
           $$PWD/src/restricao.h \
           $$PWD/src/tipo.h \
           $$PWD/src/espacotabela.h \
           $$PWD/src/gatilho.h \
           $$PWD/src/baseobject.h \
           $$PWD/src/regra.h \
           $$PWD/src/sequencia.h \
           $$PWD/src/tipospgsql.h \
           $$PWD/src/conversion.h \
           $$PWD/src/esquema.h \
           $$PWD/src/elementoindice.h \
           $$PWD/src/indice.h \
           $$PWD/src/basegraphicobject.h \
           $$PWD/src/relacionamento.h \
           $$PWD/src/tabela.h \
           $$PWD/src/visao.h \
           $$PWD/src/familiaoperadores.h \
           $$PWD/src/elemclasseoperadores.h \
           $$PWD/src/operatorclass.h \
           $$PWD/src/listaoperacoes.h \
           $$PWD/src/tableobject.h \
           $$PWD/src/referencia.h \
           $$PWD/src/permissao.h

SOURCES +=  $$PWD/src/textbox.cpp \
            $$PWD/src/domain.cpp \
            $$PWD/src/gatilho.cpp \
            $$PWD/src/basegraphicobject.cpp \
            $$PWD/src/relacionamentobase.cpp \
            $$PWD/src/tipo.cpp \
            $$PWD/src/column.cpp \
            $$PWD/src/espacotabela.cpp \
            $$PWD/src/elementoindice.cpp \
            $$PWD/src/indice.cpp \
            $$PWD/src/operador.cpp \
            $$PWD/src/restricao.cpp \
            $$PWD/src/tipospgsql.cpp \
            $$PWD/src/esquema.cpp \
            $$PWD/src/linguagem.cpp \
            $$PWD/src/papel.cpp \
            $$PWD/src/sequencia.cpp \
            $$PWD/src/visao.cpp \
            $$PWD/src/conversion.cpp \
            $$PWD/src/funcao.cpp \
            $$PWD/src/modelobd.cpp \
            $$PWD/src/regra.cpp \
            $$PWD/src/tabela.cpp \
            $$PWD/src/cast.cpp \
            $$PWD/src/funcaoagregacao.cpp \
            $$PWD/src/baseobject.cpp \
            $$PWD/src/relacionamento.cpp \
            $$PWD/src/basetable.cpp \
            $$PWD/src/familiaoperadores.cpp \
            $$PWD/src/elemclasseoperadores.cpp \
            $$PWD/src/operatorclass.cpp \
            $$PWD/src/listaoperacoes.cpp \
            $$PWD/src/tableobject.cpp \
            $$PWD/src/referencia.cpp \
            $$PWD/src/permissao.cpp
