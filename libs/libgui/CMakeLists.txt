cmake_minimum_required(VERSION 3.14)

# set the project name and version
project(gui VERSION 1.0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
#set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS
    Core
    Gui
    Widgets
    Network
    Svg
    REQUIRED
)

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS
    Core
    Gui
    Widgets
    Network
    Svg
    REQUIRED
)

set(HEADERS
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/baseform.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/guiutilsns.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/mainwindow.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/messagebox.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/pgmodelerplugin.h

    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/aggregatewidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/basefunctionwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/baseobjectwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/castwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/collationwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/columnpickerwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/columnwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/constraintwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/conversionwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/databasewidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/domainwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/elementstablewidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/elementwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/eventtriggerwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/extensionwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/foreigndatawrapperwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/foreignserverwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/functionwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/genericsqlwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/indexwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/languagewidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/operatorclasswidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/operatorfamilywidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/operatorwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/parameterwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/permissionwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/pgsqltypewidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/policywidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/procedurewidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/referencewidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/relationshipwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/rolewidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/rulewidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/schemawidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/sequencewidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/tablespacewidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/tablewidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/tagwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/textboxwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/transformwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/triggerwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/typewidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/usermappingwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/viewwidget.h

    $ENV{LIBS_LIBGUI_SRC_PATH}/src/settings/appearanceconfigwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/settings/baseconfigwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/settings/configurationform.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/settings/connectionsconfigwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/settings/generalconfigwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/settings/pluginsconfigwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/settings/relationshipconfigwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/settings/snippetsconfigwidget.h

    $ENV{LIBS_LIBGUI_SRC_PATH}/src/tools/bugreportform.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/tools/databaseexplorerwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/tools/databaseimportform.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/tools/databaseimporthelper.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/tools/datamanipulationform.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/tools/metadatahandlingform.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/tools/modeldatabasediffform.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/tools/modelexportform.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/tools/modelexporthelper.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/tools/modelfixform.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/tools/modelrestorationform.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/tools/modelsdiffhelper.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/tools/modelvalidationhelper.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/tools/modelvalidationwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/tools/objectsdiffinfo.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/tools/sqlexecutionhelper.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/tools/sqlexecutionwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/tools/sqltoolwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/tools/swapobjectsidswidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/tools/validationinfo.h

    $ENV{LIBS_LIBGUI_SRC_PATH}/src/utils/deletableitemdelegate.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/utils/htmlitemdelegate.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/utils/plaintextitemdelegate.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/utils/resultsetmodel.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/utils/syntaxhighlighter.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/utils/textblockinfo.h

    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/aboutwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/bulkdataeditwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/changelogwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/codecompletionwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/colorpickerwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/csvloadwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/customsqlwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/donatewidget.h
#    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/elementstablewidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/fileselectorwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/findreplacewidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/layersconfigwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/layerswidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/linenumberswidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/modelnavigationwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/modelobjectswidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/modeloverviewwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/modelwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/newobjectoverlaywidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/numberedtexteditor.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/objectdepsrefswidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/objectfinderwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/objectrenamewidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/objectselectorwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/objectsfilterwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/objectstablewidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/operationlistwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/sceneinfowidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/sourcecodewidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/tabledatawidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/taskprogresswidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/updatenotifierwidget.h
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/welcomewidget.h

)

set(SOURCES
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/baseform.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/guiutilsns.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/mainwindow.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/messagebox.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/pgmodelerplugin.cpp

    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/aggregatewidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/basefunctionwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/baseobjectwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/castwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/collationwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/columnpickerwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/columnwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/constraintwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/conversionwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/databasewidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/domainwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/elementstablewidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/elementwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/eventtriggerwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/extensionwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/foreigndatawrapperwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/foreignserverwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/functionwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/genericsqlwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/indexwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/languagewidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/operatorclasswidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/operatorfamilywidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/operatorwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/parameterwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/permissionwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/pgsqltypewidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/policywidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/procedurewidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/referencewidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/relationshipwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/rolewidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/rulewidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/schemawidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/sequencewidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/tablespacewidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/tablewidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/tagwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/textboxwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/transformwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/triggerwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/typewidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/usermappingwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects/viewwidget.cpp

    $ENV{LIBS_LIBGUI_SRC_PATH}/src/settings/appearanceconfigwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/settings/baseconfigwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/settings/configurationform.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/settings/connectionsconfigwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/settings/generalconfigwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/settings/pluginsconfigwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/settings/relationshipconfigwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/settings/snippetsconfigwidget.cpp

    $ENV{LIBS_LIBGUI_SRC_PATH}/src/tools/bugreportform.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/tools/databaseexplorerwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/tools/databaseimportform.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/tools/databaseimporthelper.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/tools/datamanipulationform.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/tools/metadatahandlingform.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/tools/modeldatabasediffform.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/tools/modelexportform.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/tools/modelexporthelper.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/tools/modelfixform.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/tools/modelrestorationform.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/tools/modelsdiffhelper.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/tools/modelvalidationhelper.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/tools/modelvalidationwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/tools/objectsdiffinfo.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/tools/sqlexecutionhelper.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/tools/sqlexecutionwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/tools/sqltoolwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/tools/swapobjectsidswidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/tools/validationinfo.cpp

    $ENV{LIBS_LIBGUI_SRC_PATH}/src/utils/deletableitemdelegate.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/utils/htmlitemdelegate.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/utils/plaintextitemdelegate.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/utils/resultsetmodel.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/utils/syntaxhighlighter.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/utils/textblockinfo.cpp

    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/aboutwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/bulkdataeditwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/changelogwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/codecompletionwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/colorpickerwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/csvloadwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/customsqlwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/donatewidget.cpp
#    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/elementstablewidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/fileselectorwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/findreplacewidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/layersconfigwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/layerswidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/linenumberswidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/modelnavigationwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/modelobjectswidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/modeloverviewwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/modelwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/newobjectoverlaywidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/numberedtexteditor.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/objectdepsrefswidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/objectfinderwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/objectrenamewidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/objectselectorwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/objectsfilterwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/objectstablewidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/operationlistwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/sceneinfowidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/sourcecodewidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/tabledatawidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/taskprogresswidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/updatenotifierwidget.cpp
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets/welcomewidget.cpp

)

set(FORMS
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/dbobjects/aggregatewidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/dbobjects/basefunctionwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/dbobjects/baseobjectwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/dbobjects/castwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/dbobjects/collationwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/dbobjects/columnpickerwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/dbobjects/columnwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/dbobjects/constraintwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/dbobjects/conversionwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/dbobjects/databasewidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/dbobjects/domainwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/dbobjects/elementwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/dbobjects/eventtriggerwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/dbobjects/extensionwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/dbobjects/foreigndatawrapperwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/dbobjects/foreignserverwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/dbobjects/functionwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/dbobjects/genericsqlwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/dbobjects/indexwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/dbobjects/languagewidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/dbobjects/operatorclasswidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/dbobjects/operatorfamilywidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/dbobjects/operatorwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/dbobjects/parameterwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/dbobjects/permissionwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/dbobjects/pgsqltypewidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/dbobjects/policywidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/dbobjects/referencewidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/dbobjects/relationshipwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/dbobjects/rolewidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/dbobjects/rulewidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/dbobjects/schemawidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/dbobjects/sequencewidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/dbobjects/tablespacewidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/dbobjects/tablewidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/dbobjects/tagwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/dbobjects/textboxwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/dbobjects/transformwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/dbobjects/triggerwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/dbobjects/typewidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/dbobjects/usermappingwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/dbobjects/viewwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/settings/appearanceconfigwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/settings/configurationform.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/settings/connectionsconfigwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/settings/generalconfigwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/settings/pluginsconfigwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/settings/relationshipconfigwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/settings/snippetsconfigwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/tools/bugreportform.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/tools/databaseexplorerwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/tools/databaseimportform.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/tools/datamanipulationform.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/tools/metadatahandlingform.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/tools/modeldatabasediffform.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/tools/modelexportform.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/tools/modelfixform.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/tools/modelrestorationform.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/tools/modelvalidationwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/tools/sqlexecutionwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/tools/sqltoolwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/tools/swapobjectsidswidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/widgets/aboutwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/widgets/bulkdataeditwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/widgets/changelogwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/widgets/colorpickerwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/widgets/csvloadwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/widgets/customsqlwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/widgets/donatewidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/widgets/fileselectorwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/widgets/findreplacewidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/widgets/layersconfigwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/widgets/layerswidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/widgets/modelnavigationwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/widgets/modelobjectswidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/widgets/modeloverviewwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/widgets/newobjectoverlaywidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/widgets/objectdepsrefswidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/widgets/objectfinderwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/widgets/objectrenamewidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/widgets/objectselectorwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/widgets/objectsfilterwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/widgets/objectstablewidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/widgets/operationlistwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/widgets/sceneinfowidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/widgets/sourcecodewidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/widgets/tabledatawidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/widgets/taskprogresswidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/widgets/updatenotifierwidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/widgets/welcomewidget.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/baseform.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/mainwindow.ui
    $ENV{LIBS_LIBGUI_SRC_PATH}/ui/messagebox.ui
)

QT5_WRAP_UI(UI_HEADERS ${FORMS})

set(RESOURCES
    $ENV{LIBS_LIBGUI_SRC_PATH}/res/resources.qrc
)

add_library(gui SHARED
    ${HEADERS}
    ${UI_HEADERS}
    ${SOURCES}
    ${FORMS}
    ${RESOURCES}
)

#target_compile_definitions(gui PRIVATE gui_LIBRARY)

set(INCLUDE_DIRECTORIES
    ${Qt${QT_VERSION_MAJOR}Core_INCLUDE_DIRS}
    ${Qt${QT_VERSION_MAJOR}Gui_INCLUDE_DIRS}
    ${Qt${QT_VERSION_MAJOR}Widgets_INCLUDE_DIRS}
    ${Qt${QT_VERSION_MAJOR}Network_INCLUDE_DIRS}
    ${Qt${QT_VERSION_MAJOR}Svg_INCLUDE_DIRS}
    #
    $ENV{LIBS_LIBGUI_SRC_PATH}/src
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/dbobjects
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/settings
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/tools
    $ENV{LIBS_LIBGUI_SRC_PATH}/src/widgets
    #
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/ui/dbobjects
    ${CMAKE_CURRENT_BINARY_DIR}/ui/settings
    ${CMAKE_CURRENT_BINARY_DIR}/ui/tools
    ${CMAKE_CURRENT_BINARY_DIR}/ui/widgets
    #
    $ENV{LIBS_LIBCANVAS_SRC_PATH}/src
    $ENV{LIBS_LIBCONNECTOR_SRC_PATH}/src
    $ENV{LIBS_LIBCORE_SRC_PATH}/src
    $ENV{LIBS_LIBUTILS_SRC_PATH}/src

)

target_include_directories(gui PUBLIC ${INCLUDE_DIRECTORIES})

target_link_libraries(gui PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Svg
    #
    canvas
    connector
    core
    utils
)

#Install

#install(TARGETS gui
#        CONFIGURATIONS Debug
#        DESTINATION debug/bin)

#install(TARGETS gui
#        CONFIGURATIONS Release
#        DESTINATION release/bin)
