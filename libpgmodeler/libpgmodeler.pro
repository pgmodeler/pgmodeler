# libpgmodeler.pro (reviewed version)
#
# Refactored by: Lisandro Damián Nicanor Pérez Meyer <perezmeyer@gmail.com>
# Refactored code: https://github.com/perezmeyer/pgmodeler/tree/shared_libs
# Reviewed by: Raphal Araújo e Silva <raphael@pgmodeler.com.br>
#
# NOTE: Reviewed code is not a direct merge from refactored version but based upon the
# refactored code, containing almost all changes done by the refactoring author.

include(../pgmodeler.pri)

TEMPLATE = lib
TARGET = pgmodeler
windows: DESTDIR = $$PWD

HEADERS += src/textbox.h \
	   src/cast.h \
	   src/defaultlanguages.h \
	   src/function.h \
	   src/language.h \
	   src/operator.h \
	   src/baserelationship.h \
	   src/basetable.h \
	   src/column.h \
	   src/domain.h \
	   src/aggregate.h \
	   src/permission.h \
	   src/databasemodel.h \
	   src/pgsqltypes/actiontype.h \
	   src/pgsqltypes/basetype.h \
	   src/pgsqltypes/behaviortype.h \
	   src/pgsqltypes/categorytype.h \
	   src/pgsqltypes/constrainttype.h \
	   src/pgsqltypes/deferraltype.h \
	   src/pgsqltypes/encodingtype.h \
	   src/pgsqltypes/eventtriggertype.h \
	   src/pgsqltypes/eventtype.h \
	   src/pgsqltypes/executiontype.h \
	   src/pgsqltypes/firingtype.h \
	   src/pgsqltypes/functiontype.h \
	   src/pgsqltypes/identitytype.h \
	   src/pgsqltypes/indexingtype.h \
	   src/pgsqltypes/intervaltype.h \
	   src/pgsqltypes/matchtype.h \
	   src/pgsqltypes/partitioningtype.h \
	   src/pgsqltypes/pgsqltype.h \
	   src/pgsqltypes/policycmdtype.h \
	   src/pgsqltypes/securitytype.h \
	   src/pgsqltypes/spatialtype.h \
	   src/pgsqltypes/storagetype.h \
	   src/pgsqltypes/templatetype.h \
	   src/pgsqltypes/usertypeconfig.h \
	   src/role.h \
	   src/constraint.h \
	   src/type.h \
	   src/tablespace.h \
	   src/trigger.h \
	   src/baseobject.h \
	   src/rule.h \
	   src/sequence.h \
	   src/conversion.h \
	   src/schema.h \
	   src/indexelement.h \
	   src/index.h \
	   src/basegraphicobject.h \
	   src/relationship.h \
	   src/table.h \
	   src/usermapping.h \
	   src/view.h \
	   src/operatorfamily.h \
	   src/operatorclasselement.h \
	   src/operatorclass.h \
	   src/operationlist.h \
	   src/tableobject.h \
	   src/reference.h \
	   src/collation.h \
	   src/excludeelement.h \
	   src/element.h \
	   src/parameter.h \
	   src/operation.h \
	   src/copyoptions.h \
	   src/typeattribute.h \
	   src/extension.h \
	   src/pgmodelerns.h \
	   src/tag.h \
	   src/eventtrigger.h \
	   src/genericsql.h \
	   src/policy.h \
	   src/partitionkey.h \
	   src/foreigndatawrapper.h \
	   src/foreignobject.h \
	   src/foreignserver.h \
	   src/physicaltable.h \
	   src/foreigntable.h

SOURCES +=  src/textbox.cpp \
	    src/domain.cpp \
	    src/pgsqltypes/actiontype.cpp \
	    src/pgsqltypes/basetype.cpp \
	    src/pgsqltypes/behaviortype.cpp \
	    src/pgsqltypes/categorytype.cpp \
	    src/pgsqltypes/constrainttype.cpp \
	    src/pgsqltypes/deferraltype.cpp \
	    src/pgsqltypes/encodingtype.cpp \
	    src/pgsqltypes/eventtriggertype.cpp \
	    src/pgsqltypes/eventtype.cpp \
	    src/pgsqltypes/executiontype.cpp \
	    src/pgsqltypes/firingtype.cpp \
	    src/pgsqltypes/functiontype.cpp \
	    src/pgsqltypes/identitytype.cpp \
	    src/pgsqltypes/indexingtype.cpp \
	    src/pgsqltypes/intervaltype.cpp \
	    src/pgsqltypes/matchtype.cpp \
	    src/pgsqltypes/partitioningtype.cpp \
	    src/pgsqltypes/pgsqltype.cpp \
	    src/pgsqltypes/policycmdtype.cpp \
	    src/pgsqltypes/securitytype.cpp \
	    src/pgsqltypes/spatialtype.cpp \
	    src/pgsqltypes/storagetype.cpp \
	    src/trigger.cpp \
	    src/basegraphicobject.cpp \
	    src/baserelationship.cpp \
	    src/type.cpp \
	    src/column.cpp \
	    src/tablespace.cpp \
	    src/indexelement.cpp \
	    src/index.cpp \
	    src/operator.cpp \
	    src/constraint.cpp \
	    src/schema.cpp \
	    src/language.cpp \
	    src/role.cpp \
	    src/sequence.cpp \
	    src/usermapping.cpp \
	    src/view.cpp \
	    src/conversion.cpp \
	    src/function.cpp \
	    src/permission.cpp \
	    src/databasemodel.cpp \
	    src/rule.cpp \
	    src/table.cpp \
	    src/cast.cpp \
	    src/aggregate.cpp \
	    src/baseobject.cpp \
	    src/relationship.cpp \
	    src/basetable.cpp \
	    src/operatorfamily.cpp \
	    src/operatorclasselement.cpp \
	    src/operatorclass.cpp \
	    src/operationlist.cpp \
	    src/tableobject.cpp \
	    src/reference.cpp \
	    src/collation.cpp \
	    src/excludeelement.cpp \
	    src/element.cpp \
	    src/parameter.cpp \
	    src/copyoptions.cpp \
	    src/typeattribute.cpp \
	    src/extension.cpp \
	    src/pgmodelerns.cpp \
	    src/tag.cpp \
	    src/eventtrigger.cpp \
	    src/operation.cpp \
	    src/genericsql.cpp \
	    src/policy.cpp \
	    src/partitionkey.cpp \
	    src/foreigndatawrapper.cpp \
	    src/foreignobject.cpp \
	    src/foreignserver.cpp \
	    src/physicaltable.cpp \
	    src/foreigntable.cpp

unix|windows: LIBS += -L$$OUT_PWD/../libparsers/ -lparsers \
                    -L$$OUT_PWD/../libutils/ -lutils

INCLUDEPATH += $$PWD/../libparsers/src \
               $$PWD/../libutils/src

DEPENDPATH += $$PWD/../libparsers \
              $$PWD/../libutils

# Deployment settings
target.path = $$PRIVATELIBDIR
INSTALLS = target
