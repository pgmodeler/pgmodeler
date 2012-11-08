include(../pgmodeler.pro)

TEMPLATE = lib
TARGET = pgmodeler
DESTDIR = ../build/lib

LIBS += $$DESTDIR/$$LIBUTIL \
        $$DESTDIR/$$LIBPARSERS

HEADERS += $$PWD/src/textbox.h \
           $$PWD/src/cast.h \
           $$PWD/src/function.h \
           $$PWD/src/linguagem.h \
           $$PWD/src/operador.h \
           $$PWD/src/relacionamentobase.h \
           $$PWD/src/basetable.h \
           $$PWD/src/column.h \
           $$PWD/src/domain.h \
           $$PWD/src/aggregate.h \
           $$PWD/src/modelobd.h \
           $$PWD/src/papel.h \
           $$PWD/src/restricao.h \
           $$PWD/src/tipo.h \
           $$PWD/src/tablespace.h \
           $$PWD/src/trigger.h \
           $$PWD/src/baseobject.h \
           $$PWD/src/regra.h \
           $$PWD/src/sequencia.h \
           $$PWD/src/tipospgsql.h \
           $$PWD/src/conversion.h \
           $$PWD/src/schema.h \
           $$PWD/src/indexelement.h \
           $$PWD/src/indice.h \
           $$PWD/src/basegraphicobject.h \
           $$PWD/src/relacionamento.h \
           $$PWD/src/tabela.h \
           $$PWD/src/visao.h \
           $$PWD/src/operatorfamily.h \
           $$PWD/src/operatorclasselement.h \
           $$PWD/src/operatorclass.h \
           $$PWD/src/listaoperacoes.h \
           $$PWD/src/tableobject.h \
           $$PWD/src/referencia.h \
           $$PWD/src/permissao.h


SOURCES +=  $$PWD/src/textbox.cpp \
            $$PWD/src/domain.cpp \
            $$PWD/src/trigger.cpp \
            $$PWD/src/basegraphicobject.cpp \
            $$PWD/src/relacionamentobase.cpp \
            $$PWD/src/tipo.cpp \
            $$PWD/src/column.cpp \
            $$PWD/src/tablespace.cpp \
            $$PWD/src/indexelement.cpp \
            $$PWD/src/indice.cpp \
            $$PWD/src/operador.cpp \
            $$PWD/src/restricao.cpp \
            $$PWD/src/tipospgsql.cpp \
            $$PWD/src/schema.cpp \
            $$PWD/src/linguagem.cpp \
            $$PWD/src/papel.cpp \
            $$PWD/src/sequencia.cpp \
            $$PWD/src/visao.cpp \
            $$PWD/src/conversion.cpp \
            $$PWD/src/function.cpp \
            $$PWD/src/modelobd.cpp \
            $$PWD/src/regra.cpp \
            $$PWD/src/tabela.cpp \
            $$PWD/src/cast.cpp \
            $$PWD/src/aggregate.cpp \
            $$PWD/src/baseobject.cpp \
            $$PWD/src/relacionamento.cpp \
            $$PWD/src/basetable.cpp \
            $$PWD/src/operatorfamily.cpp \
            $$PWD/src/operatorclasselement.cpp \
            $$PWD/src/operatorclass.cpp \
            $$PWD/src/listaoperacoes.cpp \
            $$PWD/src/tableobject.cpp \
            $$PWD/src/referencia.cpp \
            $$PWD/src/permissao.cpp
