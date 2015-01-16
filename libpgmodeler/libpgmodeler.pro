include(../pgmodeler.pri)

TEMPLATE = lib
TARGET = pgmodeler

HEADERS += src/textbox.h \
	   src/cast.h \
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
	   src/role.h \
	   src/constraint.h \
	   src/type.h \
	   src/tablespace.h \
	   src/trigger.h \
	   src/baseobject.h \
	   src/rule.h \
	   src/sequence.h \
	   src/pgsqltypes.h \
	   src/conversion.h \
	   src/schema.h \
	   src/indexelement.h \
	   src/index.h \
	   src/basegraphicobject.h \
	   src/relationship.h \
	   src/table.h \
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
           src/eventtrigger.h


SOURCES +=  src/textbox.cpp \
	    src/domain.cpp \
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
	    src/pgsqltypes.cpp \
	    src/schema.cpp \
	    src/language.cpp \
	    src/role.cpp \
	    src/sequence.cpp \
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
            src/eventtrigger.cpp

unix|win32: LIBS += -L$$OUT_PWD/../libparsers/ -lparsers \
                    -L$$OUT_PWD/../libutils/ -lutils

INCLUDEPATH += $$PWD/../libparsers/src \
               $$PWD/../libutils/src

DEPENDPATH += $$PWD/../libparsers \
              $$PWD/../libutils

# Deployment settings
target.path = $$PRIVATELIBDIR
INSTALLS = target
