include(../pgmodeler.pro)

TEMPLATE = lib
TARGET = pgmodeler
DESTDIR = ../build/lib

LIBS += $$DESTDIR/$$LIBUTIL \
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
           $$PWD/src/modelobd.h \
           $$PWD/src/role.h \
           $$PWD/src/constraint.h \
           $$PWD/src/tipo.h \
           $$PWD/src/tablespace.h \
           $$PWD/src/trigger.h \
           $$PWD/src/baseobject.h \
           $$PWD/src/rule.h \
           $$PWD/src/sequence.h \
           $$PWD/src/tipospgsql.h \
           $$PWD/src/conversion.h \
           $$PWD/src/schema.h \
           $$PWD/src/indexelement.h \
           $$PWD/src/index.h \
           $$PWD/src/basegraphicobject.h \
           $$PWD/src/relationship.h \
           $$PWD/src/tabela.h \
           $$PWD/src/visao.h \
           $$PWD/src/operatorfamily.h \
           $$PWD/src/operatorclasselement.h \
           $$PWD/src/operatorclass.h \
           $$PWD/src/operationlist.h \
           $$PWD/src/tableobject.h \
           $$PWD/src/reference.h \
           $$PWD/src/permission.h


SOURCES +=  $$PWD/src/textbox.cpp \
            $$PWD/src/domain.cpp \
            $$PWD/src/trigger.cpp \
            $$PWD/src/basegraphicobject.cpp \
            $$PWD/src/baserelationship.cpp \
            $$PWD/src/tipo.cpp \
            $$PWD/src/column.cpp \
            $$PWD/src/tablespace.cpp \
            $$PWD/src/indexelement.cpp \
            $$PWD/src/index.cpp \
            $$PWD/src/operator.cpp \
            $$PWD/src/constraint.cpp \
            $$PWD/src/tipospgsql.cpp \
            $$PWD/src/schema.cpp \
            $$PWD/src/language.cpp \
            $$PWD/src/role.cpp \
            $$PWD/src/sequence.cpp \
            $$PWD/src/visao.cpp \
            $$PWD/src/conversion.cpp \
            $$PWD/src/function.cpp \
            $$PWD/src/modelobd.cpp \
            $$PWD/src/rule.cpp \
            $$PWD/src/tabela.cpp \
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
            $$PWD/src/permission.cpp
