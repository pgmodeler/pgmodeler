include(../pgmodeler.pro)

TEMPLATE = lib
TARGET = objrenderer

LIBS += $$DESTDIR/$$LIBUTILS \
	$$DESTDIR/$$LIBPARSERS \
	$$DESTDIR/$$LIBPGMODELER

HEADERS +=  $$PWD/src/baseobjectview.h \
            $$PWD/src/textboxview.h \
            $$PWD/src/tableview.h \
            $$PWD/src/graphicalview.h \
            $$PWD/src/relationshipview.h \
            $$PWD/src/tabletitleview.h \
            $$PWD/src/tableobjectview.h \
            $$PWD/src/basetableview.h \
            $$PWD/src/objectsscene.h \
	    $$PWD/src/schemaview.h

SOURCES +=  $$PWD/src/baseobjectview.cpp \
            $$PWD/src/textboxview.cpp \
            $$PWD/src/tableview.cpp \
            $$PWD/src/graphicalview.cpp \
            $$PWD/src/relationshipview.cpp \
            $$PWD/src/tabletitleview.cpp \
            $$PWD/src/tableobjectview.cpp \
            $$PWD/src/basetableview.cpp \
            $$PWD/src/objectsscene.cpp \
	    $$PWD/src/schemaview.cpp
