include(../pgmodeler.pro)

TEMPLATE = lib
TARGET = pgmodeler_ui
DESTDIR = ../build/lib

LIBS = $$DESTDIR/$$LIBUTIL \
       $$DESTDIR/$$LIBPARSERS \
       $$DESTDIR/$$LIBDBCONNECT \
       $$DESTDIR/$$LIBOBJRENDERER \
       $$DESTDIR/$$LIBPGMODELER

RESOURCES += res/resources.qrc
windows:RCC_DIR += src

SOURCES += $$PWD/src/formprincipal.cpp \
           $$PWD/src/modelowidget.cpp \
           $$PWD/src/formsobre.cpp \
           $$PWD/src/messagebox.cpp \
           $$PWD/src/textboxwidget.cpp \
           $$PWD/src/baseobjectwidget.cpp \
           $$PWD/src/listaoperacoeswidget.cpp \
           $$PWD/src/visaoobjetoswidget.cpp \
           $$PWD/src/formbasico.cpp \
           $$PWD/src/codigofontewidget.cpp \
           $$PWD/src/destaquesintaxe.cpp \
           $$PWD/src/databasewidget.cpp \
           $$PWD/src/esquemawidget.cpp \
           $$PWD/src/tabelaobjetoswidget.cpp \
           $$PWD/src/papelwidget.cpp \
           $$PWD/src/permissaowidget.cpp \
           $$PWD/src/espacotabelawidget.cpp \
           $$PWD/src/linguagemwidget.cpp \
           $$PWD/src/seletorobjetowidget.cpp \
           $$PWD/src/funcaowidget.cpp \
           $$PWD/src/tipopgsqlwidget.cpp \
           $$PWD/src/parametrowidget.cpp \
           $$PWD/src/conversaotipowidget.cpp \
           $$PWD/src/conversaocodificacaowidget.cpp \
           $$PWD/src/dominiowidget.cpp \
           $$PWD/src/funcaoagregacaowidget.cpp \
           $$PWD/src/sequenciawidget.cpp \
           $$PWD/src/operadorwidget.cpp \
           $$PWD/src/familiaoperadoreswidget.cpp \
           $$PWD/src/operatorclasswidget.cpp \
           $$PWD/src/tipowidget.cpp \
           $$PWD/src/visaowidget.cpp \
           $$PWD/src/colunawidget.cpp \
           $$PWD/src/restricaowidget.cpp \
           $$PWD/src/regrawidget.cpp \
           $$PWD/src/gatilhowidget.cpp \
           $$PWD/src/indicewidget.cpp \
           $$PWD/src/relacionamentowidget.cpp \
           $$PWD/src/tabelawidget.cpp \
           $$PWD/src/progressotarefa.cpp \
           $$PWD/src/listaobjetoswidget.cpp \
           $$PWD/src/confgeralwidget.cpp \
           $$PWD/src/confaparenciawidget.cpp \
           $$PWD/src/confconexoeswidget.cpp \
           $$PWD/src/formconfiguracao.cpp \
           $$PWD/src/confbasewidget.cpp \
           $$PWD/src/formexportacao.cpp \
           $$PWD/src/visaogeralwidget.cpp \
           $$PWD/src/formrestauracaomodelo.cpp \
           $$PWD/src/quickrenamewidget.cpp

HEADERS += $$PWD/src/formprincipal.h \
           $$PWD/src/modelowidget.h \
           $$PWD/src/formsobre.h \
           $$PWD/src/messagebox.h \
           $$PWD/src/baseobjectwidget.h \
           $$PWD/src/textboxwidget.h \
           $$PWD/src/listaoperacoeswidget.h \
           $$PWD/src/visaoobjetoswidget.h \
           $$PWD/src/formbasico.h \
           $$PWD/src/codigofontewidget.h \
           $$PWD/src/destaquesintaxe.h \
           $$PWD/src/databasewidget.h \
           $$PWD/src/esquemawidget.h \
           $$PWD/src/tabelaobjetoswidget.h \
           $$PWD/src/papelwidget.h \
           $$PWD/src/permissaowidget.h \
           $$PWD/src/espacotabelawidget.h \
           $$PWD/src/linguagemwidget.h \
           $$PWD/src/seletorobjetowidget.h \
           $$PWD/src/funcaowidget.h \
           $$PWD/src/tipopgsqlwidget.h \
           $$PWD/src/parametrowidget.h \
           $$PWD/src/conversaotipowidget.h \
           $$PWD/src/conversaocodificacaowidget.h \
           $$PWD/src/dominiowidget.h \
           $$PWD/src/funcaoagregacaowidget.h \
           $$PWD/src/sequenciawidget.h \
           $$PWD/src/operadorwidget.h \
           $$PWD/src/familiaoperadoreswidget.h \
           $$PWD/src/operatorclasswidget.h \
           $$PWD/src/tipowidget.h \
           $$PWD/src/visaowidget.h \
           $$PWD/src/colunawidget.h \
           $$PWD/src/restricaowidget.h \
           $$PWD/src/regrawidget.h \
           $$PWD/src/gatilhowidget.h \
           $$PWD/src/indicewidget.h \
           $$PWD/src/relacionamentowidget.h \
           $$PWD/src/tabelawidget.h \
           $$PWD/src/progressotarefa.h \
           $$PWD/src/listaobjetoswidget.h \
           $$PWD/src/confgeralwidget.h \
           $$PWD/src/formconfiguracao.h \
           $$PWD/src/confconexoeswidget.h \
           $$PWD/src/confaparenciawidget.h \
           $$PWD/src/confbasewidget.h \
           $$PWD/src/formexportacao.h \
           $$PWD/src/pgmodelerplugin.h \
           $$PWD/src/visaogeralwidget.h \
           $$PWD/src/formrestauracaomodelo.h \
           $$PWD/src/quickrenamewidget.h

FORMS += $$PWD/ui/formprincipal.ui \
         $$PWD/ui/formsobre.ui \
         $$PWD/ui/textboxwidget.ui \
         $$PWD/ui/messagebox.ui \
         $$PWD/ui/listaoperacoeswidget.ui \
         $$PWD/ui/visaoobjetoswidget.ui \
         $$PWD/ui/formbasico.ui \
         $$PWD/ui/codigofontewidget.ui \
         $$PWD/ui/databasewidget.ui \
         $$PWD/ui/baseobjectwidget.ui \
         $$PWD/ui/tabelaobjetoswidget.ui \
         $$PWD/ui/papelwidget.ui \
         $$PWD/ui/permissaowidget.ui \
         $$PWD/ui/espacotabelawidget.ui \
         $$PWD/ui/linguagemwidget.ui \
         $$PWD/ui/seletorobjetowidget.ui \
         $$PWD/ui/funcaowidget.ui \
         $$PWD/ui/tipopgsqlwidget.ui \
         $$PWD/ui/parametrowidget.ui \
         $$PWD/ui/conversaotipowidget.ui \
         $$PWD/ui/conversaocodificacaowidget.ui \
         $$PWD/ui/dominiowidget.ui \
         $$PWD/ui/funcaoagregacaowidget.ui \
         $$PWD/ui/sequenciawidget.ui \
         $$PWD/ui/operadorwidget.ui \
         $$PWD/ui/familiaoperadoreswidget.ui \
         $$PWD/ui/operatorclasswidget.ui \
         $$PWD/ui/tipowidget.ui \
         $$PWD/ui/visaowidget.ui \
         $$PWD/ui/colunawidget.ui \
         $$PWD/ui/restricaowidget.ui \
         $$PWD/ui/regrawidget.ui \
         $$PWD/ui/gatilhowidget.ui \
         $$PWD/ui/indicewidget.ui \
         $$PWD/ui/relacionamentowidget.ui \
         $$PWD/ui/tabelawidget.ui \
         $$PWD/ui/progressotarefa.ui \
         $$PWD/ui/listaobjetoswidget.ui \
         $$PWD/ui/formexportacao.ui \
         $$PWD/ui/formconfiguracao.ui \
         $$PWD/ui/confgeralwidget.ui \
         $$PWD/ui/confaparenciawidget.ui \
         $$PWD/ui/confconexoeswidget.ui \
         $$PWD/ui/visaogeralwidget.ui \
         $$PWD/ui/formrestauracaomodelo.ui \
         $$PWD/ui/quickrenamewidget.ui
