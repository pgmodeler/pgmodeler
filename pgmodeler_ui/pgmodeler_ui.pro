include(../pgmodeler.pro)
include(../libutil/libutil.pro)
include(../libconexbd/libconexbd.pro)
include(../libparsers/libparsers.pro)
include(../libpgmodeler/libpgmodeler.pro)
include(../libobjrenderer/libobjrenderer.pro)

CONFIG += qt warn_on uitools uic4
QT = core gui qt3support
TEMPLATE = app
TARGET = pgmodeler

TRANSLATIONS = ../lang/pgmodeler.en_US.ts

DEPENDPATH = ". res src ui moc obj"
MOC_DIR = moc
OBJECTS_DIR = obj
UI_DIR = src
DESTDIR = ../build
CODECFORTR = UTF-8
RESOURCES += res/resources.qrc

SOURCES += src/main.cpp \
           src/formprincipal.cpp \
           src/modelowidget.cpp \
           src/formsobre.cpp \
           src/caixamensagem.cpp \
           src/caixatextowidget.cpp \
           src/objetobasewidget.cpp \
           src/listaoperacoeswidget.cpp \
           src/visaoobjetoswidget.cpp \
           src/formbasico.cpp \
           src/codigofontewidget.cpp \
           src/destaquesintaxe.cpp \
           src/bancodadoswidget.cpp \
           src/esquemawidget.cpp \
           src/tabelaobjetoswidget.cpp \
           src/papelwidget.cpp \
           src/permissaowidget.cpp \
           src/espacotabelawidget.cpp \
           src/linguagemwidget.cpp \
           src/seletorobjetowidget.cpp \
           src/funcaowidget.cpp \
           src/tipopgsqlwidget.cpp \
           src/parametrowidget.cpp \
           src/conversaotipowidget.cpp \
           src/conversaocodificacaowidget.cpp \
           src/dominiowidget.cpp \
           src/funcaoagregacaowidget.cpp \
           src/sequenciawidget.cpp \
           src/operadorwidget.cpp \
           src/familiaoperadoreswidget.cpp \
           src/classeoperadoreswidget.cpp \
           src/tipowidget.cpp \
           src/visaowidget.cpp \
           src/colunawidget.cpp \
           src/restricaowidget.cpp \
           src/regrawidget.cpp \
           src/gatilhowidget.cpp \
           src/indicewidget.cpp \
           src/relacionamentowidget.cpp \
           src/tabelawidget.cpp \
           src/progressotarefa.cpp \
           src/listaobjetoswidget.cpp \
           src/confgeralwidget.cpp \
           src/confaparenciawidget.cpp \
           src/confconexoeswidget.cpp \
           src/formconfiguracao.cpp \
    src/confbasewidget.cpp \
    src/formexportacao.cpp

HEADERS += src/formprincipal.h \
           src/modelowidget.h \
           src/formsobre.h \
           src/caixamensagem.h \
           src/aplicacao.h \
           src/objetobasewidget.h \
           src/caixatextowidget.h \
           src/listaoperacoeswidget.h \
           src/visaoobjetoswidget.h \
           src/formbasico.h \
           src/codigofontewidget.h \
           src/destaquesintaxe.h \
           src/bancodadoswidget.h \
           src/esquemawidget.h \
           src/tabelaobjetoswidget.h \
           src/papelwidget.h \
           src/permissaowidget.h \
           src/espacotabelawidget.h \
           src/linguagemwidget.h \
           src/seletorobjetowidget.h \
           src/funcaowidget.h \
           src/tipopgsqlwidget.h \
           src/parametrowidget.h \
           src/conversaotipowidget.h \
           src/conversaocodificacaowidget.h \
           src/dominiowidget.h \
           src/funcaoagregacaowidget.h \
           src/sequenciawidget.h \
           src/operadorwidget.h \
           src/familiaoperadoreswidget.h \
           src/classeoperadoreswidget.h \
           src/tipowidget.h \
           src/visaowidget.h \
           src/colunawidget.h \
           src/restricaowidget.h \
           src/regrawidget.h \
           src/gatilhowidget.h \
           src/indicewidget.h \
           src/relacionamentowidget.h \
           src/tabelawidget.h \
           src/progressotarefa.h \
           src/listaobjetoswidget.h \
           src/confgeralwidget.h \
           src/formconfiguracao.h \
    src/confconexoeswidget.h \
    src/confaparenciawidget.h \
    src/confbasewidget.h \
    src/formexportacao.h

FORMS += ui/formprincipal.ui \
         ui/formsobre.ui \
         ui/caixatextowidget.ui \
         ui/caixamensagem.ui \
         ui/listaoperacoeswidget.ui \
         ui/visaoobjetoswidget.ui \
         ui/formbasico.ui \
         ui/codigofontewidget.ui \
         ui/bancodadoswidget.ui \
         ui/objetobasewidget.ui \
         ui/tabelaobjetoswidget.ui \
         ui/papelwidget.ui \
         ui/permissaowidget.ui \
         ui/espacotabelawidget.ui \
         ui/linguagemwidget.ui \
         ui/seletorobjetowidget.ui \
         ui/funcaowidget.ui \
         ui/tipopgsqlwidget.ui \
         ui/parametrowidget.ui \
         ui/conversaotipowidget.ui \
         ui/conversaocodificacaowidget.ui \
         ui/dominiowidget.ui \
         ui/funcaoagregacaowidget.ui \
         ui/sequenciawidget.ui \
         ui/operadorwidget.ui \
         ui/familiaoperadoreswidget.ui \
         ui/classeoperadoreswidget.ui \
         ui/tipowidget.ui \
         ui/visaowidget.ui \
         ui/colunawidget.ui \
         ui/restricaowidget.ui \
         ui/regrawidget.ui \
         ui/gatilhowidget.ui \
         ui/indicewidget.ui \
         ui/relacionamentowidget.ui \
         ui/tabelawidget.ui \
         ui/progressotarefa.ui \
         ui/listaobjetoswidget.ui \
         ui/formexportacao.ui \
         ui/formconfiguracao.ui \
         ui/confgeralwidget.ui \
         ui/confaparenciawidget.ui \
         ui/confconexoeswidget.ui
