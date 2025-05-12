qt_add_library(core SHARED
    src/aggregate.cpp src/aggregate.h
    src/basefunction.cpp src/basefunction.h
    src/basegraphicobject.cpp src/basegraphicobject.h
    src/baseobject.cpp src/baseobject.h
    src/baserelationship.cpp src/baserelationship.h
    src/basetable.cpp src/basetable.h
    src/cast.cpp src/cast.h
    src/collation.cpp src/collation.h
    src/column.cpp src/column.h
    src/constraint.cpp src/constraint.h
    src/conversion.cpp src/conversion.h
    src/copyoptions.cpp src/copyoptions.h
    src/coreglobal.h
    src/coreutilsns.cpp src/coreutilsns.h
    src/databasemodel.cpp src/databasemodel.h
    src/defaultlanguages.h
    src/domain.cpp src/domain.h
    src/element.cpp src/element.h
    src/eventtrigger.cpp src/eventtrigger.h
    src/excludeelement.cpp src/excludeelement.h
    src/extension.cpp src/extension.h
    src/foreigndatawrapper.cpp src/foreigndatawrapper.h
    src/foreignobject.cpp src/foreignobject.h
    src/foreignserver.cpp src/foreignserver.h
    src/foreigntable.cpp src/foreigntable.h
    src/function.cpp src/function.h
    src/genericsql.cpp src/genericsql.h
    src/index.cpp src/index.h
    src/indexelement.cpp src/indexelement.h
    src/language.cpp src/language.h
    src/operation.cpp src/operation.h
    src/operationlist.cpp src/operationlist.h
    src/operator.cpp src/operator.h
    src/operatorclass.cpp src/operatorclass.h
    src/operatorclasselement.cpp src/operatorclasselement.h
    src/operatorfamily.cpp src/operatorfamily.h
    src/parameter.cpp src/parameter.h
    src/partitionkey.cpp src/partitionkey.h
    src/permission.cpp src/permission.h
    src/pgsqltypes/actiontype.cpp src/pgsqltypes/actiontype.h
    src/pgsqltypes/behaviortype.cpp src/pgsqltypes/behaviortype.h
    src/pgsqltypes/categorytype.cpp src/pgsqltypes/categorytype.h
    src/pgsqltypes/checkoptiontype.cpp src/pgsqltypes/checkoptiontype.h
    src/pgsqltypes/constrainttype.cpp src/pgsqltypes/constrainttype.h
    src/pgsqltypes/deferraltype.cpp src/pgsqltypes/deferraltype.h
    src/pgsqltypes/encodingtype.cpp src/pgsqltypes/encodingtype.h
    src/pgsqltypes/eventtriggertype.cpp src/pgsqltypes/eventtriggertype.h
    src/pgsqltypes/eventtype.cpp src/pgsqltypes/eventtype.h
    src/pgsqltypes/executiontype.cpp src/pgsqltypes/executiontype.h
    src/pgsqltypes/firingtype.cpp src/pgsqltypes/firingtype.h
    src/pgsqltypes/functiontype.cpp src/pgsqltypes/functiontype.h
    src/pgsqltypes/identitytype.cpp src/pgsqltypes/identitytype.h
    src/pgsqltypes/indexingtype.cpp src/pgsqltypes/indexingtype.h
    src/pgsqltypes/intervaltype.cpp src/pgsqltypes/intervaltype.h
    src/pgsqltypes/matchtype.cpp src/pgsqltypes/matchtype.h
    src/pgsqltypes/paralleltype.cpp src/pgsqltypes/paralleltype.h
    src/pgsqltypes/partitioningtype.cpp src/pgsqltypes/partitioningtype.h
    src/pgsqltypes/pgsqltype.cpp src/pgsqltypes/pgsqltype.h
    src/pgsqltypes/policycmdtype.cpp src/pgsqltypes/policycmdtype.h
    src/pgsqltypes/providertype.cpp src/pgsqltypes/providertype.h
    src/pgsqltypes/securitytype.cpp src/pgsqltypes/securitytype.h
    src/pgsqltypes/spatialtype.cpp src/pgsqltypes/spatialtype.h
    src/pgsqltypes/storagetype.cpp src/pgsqltypes/storagetype.h
    src/pgsqltypes/templatetype.h
    src/pgsqltypes/usertypeconfig.h
    src/physicaltable.cpp src/physicaltable.h
    src/policy.cpp src/policy.h
    src/procedure.cpp src/procedure.h
    src/reference.cpp src/reference.h
    src/relationship.cpp src/relationship.h
    src/role.cpp src/role.h
    src/rule.cpp src/rule.h
    src/schema.cpp src/schema.h
    src/sequence.cpp src/sequence.h
    src/simplecolumn.cpp src/simplecolumn.h
    src/table.cpp src/table.h
    src/tableobject.cpp src/tableobject.h
    src/tablespace.cpp src/tablespace.h
    src/tag.cpp src/tag.h
    src/textbox.cpp src/textbox.h
    src/transform.cpp src/transform.h
    src/trigger.cpp src/trigger.h
    src/type.cpp src/type.h
    src/typeattribute.cpp src/typeattribute.h
    src/usermapping.cpp src/usermapping.h
    src/view.cpp src/view.h
)
target_include_directories(core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/libparsers/src
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/libutils/src
    ../../plugins/src
)

target_compile_definitions(core PUBLIC
    BINDIR=\\\"\\\"
    BUILDDATE=\\\"\\\"
    BUILDNUM=\\\"\\\"
    CONFDIR=\\\"\\\"
    CORE_SYMBOLS
    DOCDIR=\\\"\\\"
    LANGDIR=\\\"\\\"
    PLUGINSDIR=\\\"\\\"
    PRIVATEBINDIR=\\\"\\\"
    QT_DISABLE_DEPRECATED_BEFORE=0x060400
    SAMPLESDIR=\\\"\\\"
    SCHEMASDIR=\\\"\\\"
)

target_link_libraries(core PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Network
    Qt::PrintSupport
    Qt::Svg
    Qt::Widgets
)

if(UNIX OR windows)
    target_link_libraries(core PUBLIC
        # Remove: L(${CMAKE_CURRENT_SOURCE_DIR}/libs/libparsers)
        # Remove: L(${CMAKE_CURRENT_SOURCE_DIR}/libs/libutils)
        parsers
        utils
    )
endif()

if(CONFIG___contains___debug)
    target_compile_definitions(core PUBLIC
        PGMODELER_DEBUG
    )
endif()

#if(NOT defined(NO_CHECK_CURR_VER,var))
#    target_compile_definitions(core PUBLIC
#        CHECK_CURR_VER
#    )
#endif()

if(SNAPSHOT_BUILD___equals___ON)
    target_compile_definitions(core PUBLIC
        SNAPSHOT_BUILD
    )
endif()

if(DEMO_VERSION___equals___ON)
    target_compile_definitions(core PUBLIC
        DEMO_VERSION
    )
endif()

if(NO_UPDATE_CHECK___equals___ON)
    target_compile_definitions(core PUBLIC
        NO_UPDATE_CHECK
    )
endif()

if(PRIVATE_PLUGINS___equals___ON)
    target_compile_definitions(core PUBLIC
        PRIVATE_PLUGINS_SYMBOLS
    )
endif()

if(APPIMAGE_BUILD___equals___ON AND LINUX)
    target_compile_definitions(core PUBLIC
        APPIMAGE_BUILD
    )
endif()

#if(CMAKE_BUILD_TYPE STREQUAL Debug AND LINUX)
#    target_compile_options(core
#        -Wall -Wextra -Wuninitialized
#    )
#endif()

if(MACOS)
    set_target_properties(core PROPERTIES
        MACOSX_BUNDLE FALSE
    )
endif()

install(TARGETS core
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
