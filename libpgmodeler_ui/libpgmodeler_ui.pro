include(../pgmodeler.pro)

TEMPLATE = lib
TARGET = pgmodeler_ui
RESOURCES += res/resources.qrc
windows:RCC_DIR += src

LIBS = $$DESTDIR/$$LIBUTILS \
       $$DESTDIR/$$LIBPARSERS \
       $$DESTDIR/$$LIBDBCONNECT \
       $$DESTDIR/$$LIBOBJRENDERER \
       $$DESTDIR/$$LIBPGMODELER

SOURCES += $$PWD/src/mainwindow.cpp \
	   $$PWD/src/modelwidget.cpp \
	   $$PWD/src/aboutform.cpp \
           $$PWD/src/messagebox.cpp \
           $$PWD/src/textboxwidget.cpp \
           $$PWD/src/baseobjectwidget.cpp \
	   $$PWD/src/operationlistwidget.cpp \
	   $$PWD/src/modelobjectswidget.cpp \
	   $$PWD/src/baseform.cpp \
           $$PWD/src/sourcecodewidget.cpp \
           $$PWD/src/syntaxhighlighter.cpp \
           $$PWD/src/databasewidget.cpp \
	   $$PWD/src/schemawidget.cpp \
	   $$PWD/src/objecttablewidget.cpp \
	   $$PWD/src/rolewidget.cpp \
	   $$PWD/src/permissionwidget.cpp \
	   $$PWD/src/tablespacewidget.cpp \
	   $$PWD/src/languagewidget.cpp \
	   $$PWD/src/objectselectorwidget.cpp \
	   $$PWD/src/functionwidget.cpp \
	   $$PWD/src/pgsqltypewidget.cpp \
	   $$PWD/src/parameterwidget.cpp \
           $$PWD/src/castwidget.cpp \
           $$PWD/src/conversionwidget.cpp \
	   $$PWD/src/domainwidget.cpp \
	   $$PWD/src/aggregatewidget.cpp \
	   $$PWD/src/sequencewidget.cpp \
	   $$PWD/src/operatorwidget.cpp \
	   $$PWD/src/operatorfamilywidget.cpp \
           $$PWD/src/operatorclasswidget.cpp \
	   $$PWD/src/typewidget.cpp \
	   $$PWD/src/viewwidget.cpp \
           $$PWD/src/columnwidget.cpp \
	   $$PWD/src/constraintwidget.cpp \
	   $$PWD/src/rulewidget.cpp \
	   $$PWD/src/triggerwidget.cpp \
	   $$PWD/src/indexwidget.cpp \
	   $$PWD/src/relationshipwidget.cpp \
	   $$PWD/src/tablewidget.cpp \
	   $$PWD/src/taskprogresswidget.cpp \
	   $$PWD/src/objectdepsrefswidget.cpp \
           $$PWD/src/generalconfigwidget.cpp \
           $$PWD/src/appearanceconfigwidget.cpp \
           $$PWD/src/connectionsconfigwidget.cpp \
	   $$PWD/src/configurationform.cpp \
           $$PWD/src/baseconfigwidget.cpp \
	   $$PWD/src/modelexportform.cpp \
	   $$PWD/src/modeloverviewwidget.cpp \
	   $$PWD/src/modelrestorationform.cpp \
	   $$PWD/src/objectrenamewidget.cpp \
           $$PWD/src/pgmodelerplugin.cpp \
           $$PWD/src/pluginsconfigwidget.cpp \
	   $$PWD/src/collationwidget.cpp \
	   $$PWD/src/elementswidget.cpp \
	   $$PWD/src/modelexporthelper.cpp \
	   $$PWD/src/modelvalidationwidget.cpp \
	   $$PWD/src/modelvalidationhelper.cpp \
	   $$PWD/src/validationinfo.cpp \
	   $$PWD/src/extensionwidget.cpp


HEADERS += $$PWD/src/mainwindow.h \
	   $$PWD/src/modelwidget.h \
	   $$PWD/src/aboutform.h \
           $$PWD/src/messagebox.h \
           $$PWD/src/baseobjectwidget.h \
           $$PWD/src/textboxwidget.h \
	   $$PWD/src/operationlistwidget.h \
	   $$PWD/src/modelobjectswidget.h \
	   $$PWD/src/baseform.h \
           $$PWD/src/sourcecodewidget.h \
           $$PWD/src/syntaxhighlighter.h \
           $$PWD/src/databasewidget.h \
	   $$PWD/src/schemawidget.h \
	   $$PWD/src/objecttablewidget.h \
	   $$PWD/src/rolewidget.h \
	   $$PWD/src/permissionwidget.h \
	   $$PWD/src/tablespacewidget.h \
	   $$PWD/src/languagewidget.h \
	   $$PWD/src/objectselectorwidget.h \
	   $$PWD/src/functionwidget.h \
	   $$PWD/src/pgsqltypewidget.h \
	   $$PWD/src/parameterwidget.h \
           $$PWD/src/castwidget.h \
           $$PWD/src/conversionwidget.h \
	   $$PWD/src/domainwidget.h \
	   $$PWD/src/aggregatewidget.h \
	   $$PWD/src/sequencewidget.h \
	   $$PWD/src/operatorwidget.h \
	   $$PWD/src/operatorfamilywidget.h \
           $$PWD/src/operatorclasswidget.h \
	   $$PWD/src/typewidget.h \
	   $$PWD/src/viewwidget.h \
           $$PWD/src/columnwidget.h \
	   $$PWD/src/constraintwidget.h \
	   $$PWD/src/rulewidget.h \
	   $$PWD/src/triggerwidget.h \
	   $$PWD/src/indexwidget.h \
	   $$PWD/src/relationshipwidget.h \
	   $$PWD/src/tablewidget.h \
	   $$PWD/src/taskprogresswidget.h \
	   $$PWD/src/objectdepsrefswidget.h \
           $$PWD/src/generalconfigwidget.h \
	   $$PWD/src/configurationform.h \
           $$PWD/src/connectionsconfigwidget.h \
           $$PWD/src/appearanceconfigwidget.h \
           $$PWD/src/baseconfigwidget.h \
	   $$PWD/src/modelexportform.h \
           $$PWD/src/pgmodelerplugin.h \
	   $$PWD/src/modeloverviewwidget.h \
	   $$PWD/src/modelrestorationform.h \
	   $$PWD/src/objectrenamewidget.h \
           $$PWD/src/pluginsconfigwidget.h \
	   $$PWD/src/collationwidget.h \
	   $$PWD/src/elementswidget.h \
	   $$PWD/src/modelexporthelper.h \
	   $$PWD/src/modelvalidationwidget.h \
	   $$PWD/src/modelvalidationhelper.h \
	   $$PWD/src/validationinfo.h \
	   $$PWD/src/extensionwidget.h

FORMS += $$PWD/ui/mainwindow.ui \
	 $$PWD/ui/aboutform.ui \
         $$PWD/ui/textboxwidget.ui \
         $$PWD/ui/messagebox.ui \
	 $$PWD/ui/operationlistwidget.ui \
	 $$PWD/ui/modelobjectswidget.ui \
	 $$PWD/ui/baseform.ui \
         $$PWD/ui/sourcecodewidget.ui \
         $$PWD/ui/databasewidget.ui \
         $$PWD/ui/baseobjectwidget.ui \
	 $$PWD/ui/objecttablewidget.ui \
	 $$PWD/ui/rolewidget.ui \
	 $$PWD/ui/permissionwidget.ui \
	 $$PWD/ui/tablespacewidget.ui \
	 $$PWD/ui/languagewidget.ui \
	 $$PWD/ui/objectselectorwidget.ui \
	 $$PWD/ui/functionwidget.ui \
	 $$PWD/ui/pgsqltypewidget.ui \
	 $$PWD/ui/parameterwidget.ui \
         $$PWD/ui/castwidget.ui \
	 $$PWD/ui/conversionwidget.ui \
	 $$PWD/ui/domainwidget.ui \
	 $$PWD/ui/aggregatewidget.ui \
	 $$PWD/ui/sequencewidget.ui \
	 $$PWD/ui/operatorwidget.ui \
	 $$PWD/ui/operatorfamilywidget.ui \
         $$PWD/ui/operatorclasswidget.ui \
	 $$PWD/ui/typewidget.ui \
	 $$PWD/ui/viewwidget.ui \
         $$PWD/ui/columnwidget.ui \
	 $$PWD/ui/constraintwidget.ui \
	 $$PWD/ui/rulewidget.ui \
	 $$PWD/ui/triggerwidget.ui \
	 $$PWD/ui/indexwidget.ui \
	 $$PWD/ui/relationshipwidget.ui \
	 $$PWD/ui/tablewidget.ui \
	 $$PWD/ui/taskprogresswidget.ui \
	 $$PWD/ui/objectdepsrefswidget.ui \
	 $$PWD/ui/modelexportform.ui \
	 $$PWD/ui/configurationform.ui \
         $$PWD/ui/generalconfigwidget.ui \
         $$PWD/ui/appearanceconfigwidget.ui \
         $$PWD/ui/connectionsconfigwidget.ui \
	 $$PWD/ui/modeloverviewwidget.ui \
	 $$PWD/ui/modelrestorationform.ui \
	 $$PWD/ui/objectrenamewidget.ui \
         $$PWD/ui/pluginsconfigwidget.ui \
         $$PWD/ui/schemawidget.ui \
	 $$PWD/ui/collationwidget.ui \
	 $$PWD/ui/elementswidget.ui \
	 $$PWD/ui/modelvalidationwidget.ui \
	 $$PWD/ui/extensionwidget.ui
