include(../pgmodeler.pro)

TEMPLATE = lib
TARGET = pgmodeler
DESTDIR = ../build/lib

LIBS += $$DESTDIR/$$LIBUTIL \
        $$DESTDIR/$$LIBPARSERS

HEADERS += src/caixatexto.h \
           src/conversaotipo.h \
           src/funcao.h \
           src/linguagem.h \
           src/operador.h \
           src/relacionamentobase.h \
           src/tabelabase.h \
           src/coluna.h \
           src/dominio.h \
           src/funcaoagregacao.h \
           src/modelobd.h \
           src/papel.h \
           src/restricao.h \
           src/tipo.h \
           src/espacotabela.h \
           src/gatilho.h \
           src/objetobase.h \
           src/regra.h \
           src/sequencia.h \
           src/tipospgsql.h \
           src/conversaocodificacao.h \
           src/esquema.h \
           src/elementoindice.h \
           src/indice.h \
           src/objetograficobase.h \
           src/relacionamento.h \
           src/tabela.h \
           src/visao.h \
           src/familiaoperadores.h \
           src/elemclasseoperadores.h \
           src/classeoperadores.h \
           src/listaoperacoes.h \
           src/objetotabela.h \
           src/referencia.h \
           src/permissao.h

SOURCES +=  src/caixatexto.cpp \
            src/dominio.cpp \
            src/gatilho.cpp \
            src/objetograficobase.cpp \
            src/relacionamentobase.cpp \
            src/tipo.cpp \
            src/coluna.cpp \
            src/espacotabela.cpp \
            src/elementoindice.cpp \
            src/indice.cpp \
            src/operador.cpp \
            src/restricao.cpp \
            src/tipospgsql.cpp \
            src/esquema.cpp \
            src/linguagem.cpp \
            src/papel.cpp \
            src/sequencia.cpp \
            src/visao.cpp \
            src/conversaocodificacao.cpp \
            src/funcao.cpp \
            src/modelobd.cpp \
            src/regra.cpp \
            src/tabela.cpp \
            src/conversaotipo.cpp \
            src/funcaoagregacao.cpp \
            src/objetobase.cpp \
            src/relacionamento.cpp \
            src/tabelabase.cpp \
            src/familiaoperadores.cpp \
            src/elemclasseoperadores.cpp \
            src/classeoperadores.cpp \
            src/listaoperacoes.cpp \
            src/objetotabela.cpp \
            src/referencia.cpp \
            src/permissao.cpp
