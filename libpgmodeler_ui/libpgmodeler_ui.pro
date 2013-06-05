include(../pgmodeler.pro)

DESTDIR=$$LIBDESTDIR
TEMPLATE = lib
TARGET = pgmodeler_ui
RESOURCES += res/resources.qrc
windows:RCC_DIR += src

LIBS = $$DESTDIR/$$LIBUTILS \
       $$DESTDIR/$$LIBPARSERS \
       $$DESTDIR/$$LIBDBCONNECT \
       $$DESTDIR/$$LIBOBJRENDERER \
       $$DESTDIR/$$LIBPGMODELER

SOURCES += src/mainwindow.cpp \
	   src/modelwidget.cpp \
	   src/aboutform.cpp \
	   src/messagebox.cpp \
	   src/textboxwidget.cpp \
	   src/baseobjectwidget.cpp \
	   src/operationlistwidget.cpp \
	   src/modelobjectswidget.cpp \
	   src/baseform.cpp \
	   src/sourcecodewidget.cpp \
	   src/syntaxhighlighter.cpp \
	   src/databasewidget.cpp \
	   src/schemawidget.cpp \
	   src/objecttablewidget.cpp \
	   src/rolewidget.cpp \
	   src/permissionwidget.cpp \
	   src/tablespacewidget.cpp \
	   src/languagewidget.cpp \
	   src/objectselectorwidget.cpp \
	   src/functionwidget.cpp \
	   src/pgsqltypewidget.cpp \
	   src/parameterwidget.cpp \
	   src/castwidget.cpp \
	   src/conversionwidget.cpp \
	   src/domainwidget.cpp \
	   src/aggregatewidget.cpp \
	   src/sequencewidget.cpp \
	   src/operatorwidget.cpp \
	   src/operatorfamilywidget.cpp \
	   src/operatorclasswidget.cpp \
	   src/typewidget.cpp \
	   src/viewwidget.cpp \
	   src/columnwidget.cpp \
	   src/constraintwidget.cpp \
	   src/rulewidget.cpp \
	   src/triggerwidget.cpp \
	   src/indexwidget.cpp \
	   src/relationshipwidget.cpp \
	   src/tablewidget.cpp \
	   src/taskprogresswidget.cpp \
	   src/objectdepsrefswidget.cpp \
	   src/generalconfigwidget.cpp \
	   src/appearanceconfigwidget.cpp \
	   src/connectionsconfigwidget.cpp \
	   src/configurationform.cpp \
	   src/baseconfigwidget.cpp \
	   src/modelexportform.cpp \
	   src/modeloverviewwidget.cpp \
	   src/modelrestorationform.cpp \
	   src/objectrenamewidget.cpp \
	   src/pgmodelerplugin.cpp \
	   src/pluginsconfigwidget.cpp \
	   src/collationwidget.cpp \
	   src/elementswidget.cpp \
	   src/modelexporthelper.cpp \
	   src/modelvalidationwidget.cpp \
	   src/modelvalidationhelper.cpp \
	   src/validationinfo.cpp \
	   src/extensionwidget.cpp \
	   src/objectfinderwidget.cpp


HEADERS += src/mainwindow.h \
	   src/modelwidget.h \
	   src/aboutform.h \
	   src/messagebox.h \
	   src/baseobjectwidget.h \
	   src/textboxwidget.h \
	   src/operationlistwidget.h \
	   src/modelobjectswidget.h \
	   src/baseform.h \
	   src/sourcecodewidget.h \
	   src/syntaxhighlighter.h \
	   src/databasewidget.h \
	   src/schemawidget.h \
	   src/objecttablewidget.h \
	   src/rolewidget.h \
	   src/permissionwidget.h \
	   src/tablespacewidget.h \
	   src/languagewidget.h \
	   src/objectselectorwidget.h \
	   src/functionwidget.h \
	   src/pgsqltypewidget.h \
	   src/parameterwidget.h \
	   src/castwidget.h \
	   src/conversionwidget.h \
	   src/domainwidget.h \
	   src/aggregatewidget.h \
	   src/sequencewidget.h \
	   src/operatorwidget.h \
	   src/operatorfamilywidget.h \
	   src/operatorclasswidget.h \
	   src/typewidget.h \
	   src/viewwidget.h \
	   src/columnwidget.h \
	   src/constraintwidget.h \
	   src/rulewidget.h \
	   src/triggerwidget.h \
	   src/indexwidget.h \
	   src/relationshipwidget.h \
	   src/tablewidget.h \
	   src/taskprogresswidget.h \
	   src/objectdepsrefswidget.h \
	   src/generalconfigwidget.h \
	   src/configurationform.h \
	   src/connectionsconfigwidget.h \
	   src/appearanceconfigwidget.h \
	   src/baseconfigwidget.h \
	   src/modelexportform.h \
	   src/pgmodelerplugin.h \
	   src/modeloverviewwidget.h \
	   src/modelrestorationform.h \
	   src/objectrenamewidget.h \
	   src/pluginsconfigwidget.h \
	   src/collationwidget.h \
	   src/elementswidget.h \
	   src/modelexporthelper.h \
	   src/modelvalidationwidget.h \
	   src/modelvalidationhelper.h \
	   src/validationinfo.h \
	   src/extensionwidget.h \
	   src/objectfinderwidget.h

FORMS += ui/mainwindow.ui \
	 ui/aboutform.ui \
	 ui/textboxwidget.ui \
	 ui/messagebox.ui \
	 ui/operationlistwidget.ui \
	 ui/modelobjectswidget.ui \
	 ui/baseform.ui \
	 ui/sourcecodewidget.ui \
	 ui/databasewidget.ui \
	 ui/baseobjectwidget.ui \
	 ui/objecttablewidget.ui \
	 ui/rolewidget.ui \
	 ui/permissionwidget.ui \
	 ui/tablespacewidget.ui \
	 ui/languagewidget.ui \
	 ui/objectselectorwidget.ui \
	 ui/functionwidget.ui \
	 ui/pgsqltypewidget.ui \
	 ui/parameterwidget.ui \
	 ui/castwidget.ui \
	 ui/conversionwidget.ui \
	 ui/domainwidget.ui \
	 ui/aggregatewidget.ui \
	 ui/sequencewidget.ui \
	 ui/operatorwidget.ui \
	 ui/operatorfamilywidget.ui \
	 ui/operatorclasswidget.ui \
	 ui/typewidget.ui \
	 ui/viewwidget.ui \
	 ui/columnwidget.ui \
	 ui/constraintwidget.ui \
	 ui/rulewidget.ui \
	 ui/triggerwidget.ui \
	 ui/indexwidget.ui \
	 ui/relationshipwidget.ui \
	 ui/tablewidget.ui \
	 ui/taskprogresswidget.ui \
	 ui/objectdepsrefswidget.ui \
	 ui/modelexportform.ui \
	 ui/configurationform.ui \
	 ui/generalconfigwidget.ui \
	 ui/appearanceconfigwidget.ui \
	 ui/connectionsconfigwidget.ui \
	 ui/modeloverviewwidget.ui \
	 ui/modelrestorationform.ui \
	 ui/objectrenamewidget.ui \
	 ui/pluginsconfigwidget.ui \
	 ui/schemawidget.ui \
	 ui/collationwidget.ui \
	 ui/elementswidget.ui \
	 ui/modelvalidationwidget.ui \
	 ui/extensionwidget.ui \
	 ui/objectfinderwidget.ui
