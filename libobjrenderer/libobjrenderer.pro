include(../pgmodeler.pro)

DESTDIR=$$LIBDESTDIR
TEMPLATE = lib
TARGET = objrenderer

LIBS += $$DESTDIR/$$LIBUTILS \
	$$DESTDIR/$$LIBPARSERS \
	$$DESTDIR/$$LIBPGMODELER

HEADERS +=  src/baseobjectview.h \
	    src/textboxview.h \
	    src/tableview.h \
	    src/graphicalview.h \
	    src/relationshipview.h \
	    src/tabletitleview.h \
	    src/tableobjectview.h \
	    src/basetableview.h \
	    src/objectsscene.h \
	    src/schemaview.h

SOURCES +=  src/baseobjectview.cpp \
	    src/textboxview.cpp \
	    src/tableview.cpp \
	    src/graphicalview.cpp \
	    src/relationshipview.cpp \
	    src/tabletitleview.cpp \
	    src/tableobjectview.cpp \
	    src/basetableview.cpp \
	    src/objectsscene.cpp \
	    src/schemaview.cpp
