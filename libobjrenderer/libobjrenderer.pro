include(../pgmodeler.pro)

TEMPLATE = lib
TARGET = objrenderer

# The QMAKE_LFLAGS does not work properly with MingW (Windows) or Xcode (MacOSX)
# so it's necessary to pass manually all the used libraries to the linker
windows | macx {
 LIBS += $$DESTDIR/$$LIBUTILS \
         $$DESTDIR/$$LIBPARSERS \
         $$DESTDIR/$$LIBPGMODELER
}

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
