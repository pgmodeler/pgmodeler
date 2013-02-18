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
	   $$PWD/src/operationlistwidget.cpp \
           $$PWD/src/visaoobjetoswidget.cpp \
           $$PWD/src/formbasico.cpp \
           $$PWD/src/sourcecodewidget.cpp \
           $$PWD/src/syntaxhighlighter.cpp \
           $$PWD/src/databasewidget.cpp \
	   $$PWD/src/schemawidget.cpp \
           $$PWD/src/tabelaobjetoswidget.cpp \
	   $$PWD/src/rolewidget.cpp \
	   $$PWD/src/permissionwidget.cpp \
	   $$PWD/src/tablespacewidget.cpp \
	   $$PWD/src/languagewidget.cpp \
	   $$PWD/src/objectselectorwidget.cpp \
	   $$PWD/src/functionwidget.cpp \
           $$PWD/src/tipopgsqlwidget.cpp \
	   $$PWD/src/parameterwidget.cpp \
           $$PWD/src/castwidget.cpp \
           $$PWD/src/conversionwidget.cpp \
	   $$PWD/src/domainwidget.cpp \
	   $$PWD/src/aggregatewidget.cpp \
           $$PWD/src/sequenciawidget.cpp \
	   $$PWD/src/operatorwidget.cpp \
	   $$PWD/src/operatorfamilywidget.cpp \
           $$PWD/src/operatorclasswidget.cpp \
           $$PWD/src/tipowidget.cpp \
           $$PWD/src/visaowidget.cpp \
           $$PWD/src/columnwidget.cpp \
	   $$PWD/src/constraintwidget.cpp \
	   $$PWD/src/rulewidget.cpp \
	   $$PWD/src/triggerwidget.cpp \
	   $$PWD/src/indexwidget.cpp \
	   $$PWD/src/relationshipwidget.cpp \
           $$PWD/src/tabelawidget.cpp \
	   $$PWD/src/taskprogresswidget.cpp \
	   $$PWD/src/objectdepsrefswidget.cpp \
           $$PWD/src/generalconfigwidget.cpp \
           $$PWD/src/appearanceconfigwidget.cpp \
           $$PWD/src/connectionsconfigwidget.cpp \
           $$PWD/src/formconfiguracao.cpp \
           $$PWD/src/baseconfigwidget.cpp \
           $$PWD/src/formexportacao.cpp \
           $$PWD/src/visaogeralwidget.cpp \
           $$PWD/src/formrestauracaomodelo.cpp \
           $$PWD/src/quickrenamewidget.cpp \
           $$PWD/src/pgmodelerplugin.cpp \
           $$PWD/src/pluginsconfigwidget.cpp


HEADERS += $$PWD/src/formprincipal.h \
           $$PWD/src/modelowidget.h \
           $$PWD/src/formsobre.h \
           $$PWD/src/messagebox.h \
           $$PWD/src/baseobjectwidget.h \
           $$PWD/src/textboxwidget.h \
	   $$PWD/src/operationlistwidget.h \
           $$PWD/src/visaoobjetoswidget.h \
           $$PWD/src/formbasico.h \
           $$PWD/src/sourcecodewidget.h \
           $$PWD/src/syntaxhighlighter.h \
           $$PWD/src/databasewidget.h \
	   $$PWD/src/schemawidget.h \
           $$PWD/src/tabelaobjetoswidget.h \
	   $$PWD/src/rolewidget.h \
	   $$PWD/src/permissionwidget.h \
	   $$PWD/src/tablespacewidget.h \
	   $$PWD/src/languagewidget.h \
	   $$PWD/src/objectselectorwidget.h \
	   $$PWD/src/functionwidget.h \
           $$PWD/src/tipopgsqlwidget.h \
	   $$PWD/src/parameterwidget.h \
           $$PWD/src/castwidget.h \
           $$PWD/src/conversionwidget.h \
	   $$PWD/src/domainwidget.h \
	   $$PWD/src/aggregatewidget.h \
           $$PWD/src/sequenciawidget.h \
	   $$PWD/src/operatorwidget.h \
	   $$PWD/src/operatorfamilywidget.h \
           $$PWD/src/operatorclasswidget.h \
           $$PWD/src/tipowidget.h \
           $$PWD/src/visaowidget.h \
           $$PWD/src/columnwidget.h \
	   $$PWD/src/constraintwidget.h \
	   $$PWD/src/rulewidget.h \
	   $$PWD/src/triggerwidget.h \
	   $$PWD/src/indexwidget.h \
	   $$PWD/src/relationshipwidget.h \
           $$PWD/src/tabelawidget.h \
	   $$PWD/src/taskprogresswidget.h \
	   $$PWD/src/objectdepsrefswidget.h \
           $$PWD/src/generalconfigwidget.h \
           $$PWD/src/formconfiguracao.h \
           $$PWD/src/connectionsconfigwidget.h \
           $$PWD/src/appearanceconfigwidget.h \
           $$PWD/src/baseconfigwidget.h \
           $$PWD/src/formexportacao.h \
           $$PWD/src/pgmodelerplugin.h \
           $$PWD/src/visaogeralwidget.h \
           $$PWD/src/formrestauracaomodelo.h \
           $$PWD/src/quickrenamewidget.h \
           $$PWD/src/pluginsconfigwidget.h

FORMS += $$PWD/ui/formprincipal.ui \
         $$PWD/ui/formsobre.ui \
         $$PWD/ui/textboxwidget.ui \
         $$PWD/ui/messagebox.ui \
	 $$PWD/ui/operationlistwidget.ui \
         $$PWD/ui/visaoobjetoswidget.ui \
         $$PWD/ui/formbasico.ui \
         $$PWD/ui/sourcecodewidget.ui \
         $$PWD/ui/databasewidget.ui \
         $$PWD/ui/baseobjectwidget.ui \
         $$PWD/ui/tabelaobjetoswidget.ui \
	 $$PWD/ui/rolewidget.ui \
	 $$PWD/ui/permissionwidget.ui \
	 $$PWD/ui/tablespacewidget.ui \
	 $$PWD/ui/languagewidget.ui \
	 $$PWD/ui/objectselectorwidget.ui \
	 $$PWD/ui/functionwidget.ui \
         $$PWD/ui/tipopgsqlwidget.ui \
	 $$PWD/ui/parameterwidget.ui \
         $$PWD/ui/castwidget.ui \
	 $$PWD/ui/conversionwidget.ui \
	 $$PWD/ui/domainwidget.ui \
	 $$PWD/ui/aggregatewidget.ui \
         $$PWD/ui/sequenciawidget.ui \
	 $$PWD/ui/operatorwidget.ui \
	 $$PWD/ui/operatorfamilywidget.ui \
         $$PWD/ui/operatorclasswidget.ui \
         $$PWD/ui/tipowidget.ui \
         $$PWD/ui/visaowidget.ui \
         $$PWD/ui/columnwidget.ui \
	 $$PWD/ui/constraintwidget.ui \
	 $$PWD/ui/rulewidget.ui \
	 $$PWD/ui/triggerwidget.ui \
	 $$PWD/ui/indexwidget.ui \
	 $$PWD/ui/relationshipwidget.ui \
         $$PWD/ui/tabelawidget.ui \
	 $$PWD/ui/taskprogresswidget.ui \
	 $$PWD/ui/objectdepsrefswidget.ui \
         $$PWD/ui/formexportacao.ui \
         $$PWD/ui/formconfiguracao.ui \
         $$PWD/ui/generalconfigwidget.ui \
         $$PWD/ui/appearanceconfigwidget.ui \
         $$PWD/ui/connectionsconfigwidget.ui \
         $$PWD/ui/visaogeralwidget.ui \
         $$PWD/ui/formrestauracaomodelo.ui \
         $$PWD/ui/quickrenamewidget.ui \
         $$PWD/ui/pluginsconfigwidget.ui \
         $$PWD/ui/schemawidget.ui
