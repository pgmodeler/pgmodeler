include(../pgmodeler.pro)

TEMPLATE = lib
TARGET = pgmodeler

LIBS += $$DESTDIR/$$LIBUTILS \
	$$DESTDIR/$$LIBPARSERS

HEADERS += $$PWD/src/textbox.h \
           $$PWD/src/cast.h \
           $$PWD/src/function.h \
           $$PWD/src/language.h \
           $$PWD/src/operator.h \
           $$PWD/src/baserelationship.h \
           $$PWD/src/basetable.h \
           $$PWD/src/column.h \
           $$PWD/src/domain.h \
           $$PWD/src/aggregate.h \
           $$PWD/src/databasemodel.h \
           $$PWD/src/role.h \
           $$PWD/src/constraint.h \
           $$PWD/src/type.h \
           $$PWD/src/tablespace.h \
           $$PWD/src/trigger.h \
           $$PWD/src/baseobject.h \
           $$PWD/src/rule.h \
           $$PWD/src/sequence.h \
           $$PWD/src/pgsqltypes.h \
           $$PWD/src/conversion.h \
           $$PWD/src/schema.h \
           $$PWD/src/indexelement.h \
           $$PWD/src/index.h \
           $$PWD/src/basegraphicobject.h \
           $$PWD/src/relationship.h \
           $$PWD/src/table.h \
           $$PWD/src/view.h \
           $$PWD/src/operatorfamily.h \
           $$PWD/src/operatorclasselement.h \
           $$PWD/src/operatorclass.h \
           $$PWD/src/operationlist.h \
           $$PWD/src/tableobject.h \
           $$PWD/src/reference.h \
           $$PWD/src/permission.h \
	   $$PWD/src/collation.h \
	   $$PWD/src/pgmodeler.h \
	   $$PWD/src/excludeelement.h \
	   $$PWD/src/element.h \
	   $$PWD/src/parameter.h \
	   $$PWD/src/operation.h \
	   $$PWD/src/copyoptions.h \
	   $$PWD/src/typeattribute.h \
    src/extension.h


SOURCES +=  $$PWD/src/textbox.cpp \
            $$PWD/src/domain.cpp \
            $$PWD/src/trigger.cpp \
            $$PWD/src/basegraphicobject.cpp \
            $$PWD/src/baserelationship.cpp \
            $$PWD/src/type.cpp \
            $$PWD/src/column.cpp \
            $$PWD/src/tablespace.cpp \
            $$PWD/src/indexelement.cpp \
            $$PWD/src/index.cpp \
            $$PWD/src/operator.cpp \
            $$PWD/src/constraint.cpp \
            $$PWD/src/pgsqltypes.cpp \
            $$PWD/src/schema.cpp \
            $$PWD/src/language.cpp \
            $$PWD/src/role.cpp \
            $$PWD/src/sequence.cpp \
            $$PWD/src/view.cpp \
            $$PWD/src/conversion.cpp \
            $$PWD/src/function.cpp \
            $$PWD/src/databasemodel.cpp \
            $$PWD/src/rule.cpp \
            $$PWD/src/table.cpp \
            $$PWD/src/cast.cpp \
            $$PWD/src/aggregate.cpp \
            $$PWD/src/baseobject.cpp \
            $$PWD/src/relationship.cpp \
            $$PWD/src/basetable.cpp \
            $$PWD/src/operatorfamily.cpp \
            $$PWD/src/operatorclasselement.cpp \
            $$PWD/src/operatorclass.cpp \
            $$PWD/src/operationlist.cpp \
            $$PWD/src/tableobject.cpp \
            $$PWD/src/reference.cpp \
            $$PWD/src/permission.cpp \
	    $$PWD/src/collation.cpp \
	    $$PWD/src/pgmodeler.cpp \
	    $$PWD/src/excludeelement.cpp \
	    $$PWD/src/element.cpp \
	    $$PWD/src/parameter.cpp \
	    $$PWD/src/copyoptions.cpp \
	    $$PWD/src/typeattribute.cpp \
    src/extension.cpp
