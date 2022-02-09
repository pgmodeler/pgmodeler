cmake_minimum_required(VERSION 3.14)

# set the project name and version
project(core VERSION 1.0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS
    Core
    Gui
    Widgets
    REQUIRED
)

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS
    Core
    Gui
    Widgets
    REQUIRED
)

find_package(LibXml2 REQUIRED)

set(HEADERS
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/aggregate.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/basefunction.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/basegraphicobject.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/baseobject.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/baserelationship.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/basetable.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/cast.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/collation.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/column.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/constraint.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/conversion.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/copyoptions.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/coreutilsns.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/databasemodel.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/defaultlanguages.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/domain.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/element.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/eventtrigger.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/excludeelement.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/extension.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/foreigndatawrapper.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/foreignobject.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/foreignserver.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/foreigntable.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/function.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/genericsql.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/indexelement.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/index.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/language.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/operation.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/operationlist.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/operatorclasselement.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/operatorclass.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/operatorfamily.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/operator.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/parameter.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/partitionkey.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/permission.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/physicaltable.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/policy.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/procedure.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/reference.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/relationship.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/role.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/rule.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/schema.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/sequence.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/simplecolumn.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/table.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/tableobject.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/tablespace.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/tag.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/textbox.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/transform.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/trigger.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/typeattribute.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/type.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/usermapping.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/view.h

    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/actiontype.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/basetype.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/behaviortype.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/categorytype.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/constrainttype.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/deferraltype.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/encodingtype.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/eventtriggertype.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/eventtype.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/executiontype.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/firingtype.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/functiontype.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/identitytype.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/indexingtype.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/intervaltype.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/matchtype.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/paralleltype.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/partitioningtype.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/pgsqltype.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/policycmdtype.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/providertype.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/securitytype.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/spatialtype.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/storagetype.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/templatetype.h
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/usertypeconfig.h
)

set(SOURCES
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/aggregate.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/basefunction.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/basegraphicobject.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/baseobject.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/baserelationship.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/basetable.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/cast.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/collation.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/column.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/constraint.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/conversion.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/copyoptions.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/coreutilsns.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/databasemodel.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/domain.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/element.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/eventtrigger.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/excludeelement.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/extension.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/foreigndatawrapper.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/foreignobject.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/foreignserver.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/foreigntable.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/function.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/genericsql.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/index.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/indexelement.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/language.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/operation.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/operationlist.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/operatorclass.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/operatorclasselement.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/operator.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/operatorfamily.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/parameter.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/partitionkey.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/permission.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/physicaltable.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/policy.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/procedure.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/reference.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/relationship.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/role.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/rule.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/schema.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/sequence.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/table.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/tableobject.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/tablespace.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/tag.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/textbox.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/transform.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/trigger.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/typeattribute.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/type.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/usermapping.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/view.cpp

    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/actiontype.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/basetype.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/behaviortype.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/categorytype.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/constrainttype.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/deferraltype.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/encodingtype.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/eventtriggertype.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/eventtype.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/executiontype.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/firingtype.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/functiontype.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/identitytype.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/indexingtype.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/intervaltype.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/matchtype.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/paralleltype.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/partitioningtype.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/pgsqltype.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/policycmdtype.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/providertype.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/securitytype.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/spatialtype.cpp
    $ENV{LIBS_LIBCORE_SRC_PATH}/src/pgsqltypes/storagetype.cpp
)

add_library(core SHARED ${HEADERS} ${SOURCES})

#target_compile_definitions(xxx PRIVATE XXX_LIBRARY)

set(INCLUDE_DIRECTORIES
    ${Qt${QT_VERSION_MAJOR}Core_INCLUDE_DIRS}
    ${Qt${QT_VERSION_MAJOR}Gui_INCLUDE_DIRS}
    ${Qt${QT_VERSION_MAJOR}Widgets_INCLUDE_DIRS}
    #
    $ENV{LIBS_LIBCORE_SRC_PATH}/src
    $ENV{LIBS_LIBPARSERS_SRC_PATH}/src
    $ENV{LIBS_LIBUTILS_SRC_PATH}/src
    #
    ${LIBXML2_INCLUDE_DIR}
)

target_include_directories(core PUBLIC ${INCLUDE_DIRECTORIES})

target_link_libraries(core PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    #
    parsers

)

#Install

#install(TARGETS core
#        CONFIGURATIONS Debug
#        DESTINATION debug/bin)

#install(TARGETS core
#        CONFIGURATIONS Release
#        DESTINATION release/bin)
