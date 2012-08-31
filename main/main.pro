include(../pgmodeler.pro)

CONFIG += qt warn_on uitools uic4
QT = core gui qt3support
TEMPLATE = app
TARGET = pgmodeler

DEPENDPATH = ". moc obj"
OBJECTS_DIR = obj
DESTDIR = ../build/

QMAKE_POST_LINK+= "cp -r ../schemas/ $$DESTDIR; \
                   cp -r ../conf/ $$DESTDIR; \
                   cp -r ../lang/ $$DESTDIR; \
                   cp ../*.md $$DESTDIR; \
                   cp ../LICENSE $$DESTDIR;"

QMAKE_DISTCLEAN+= "-r $$DESTDIR/schemas \
                      $$DESTDIR/conf \
                      $$DESTDIR/lang \
                      $$DESTDIR/*.md \
                      $$DESTDIR/LICENSE"

LIBS = $$DESTDIR/lib/$$LIBUTIL \
       $$DESTDIR/lib/$$LIBPARSERS \
       $$DESTDIR/lib/$$LIBCONEXBD \
       $$DESTDIR/lib/$$LIBOBJRENDERER \
       $$DESTDIR/lib/$$LIBPGMODELER \
       $$DESTDIR/lib/$$LIBPGMODELERUI

HEADERS += src/aplicacao.h
SOURCES += src/main.cpp
