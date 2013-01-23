PGMODELER_SRC_DIR=../../
PGMODELER_LIB_DIR=$$PGMODELER_SRC_DIR/build/lib
PGMODELER_PLUGIN_DIR=$$PGMODELER_SRC_DIR/build/plugins

!exists($$PGMODELER_SRC_DIR) {
 warning("pt_BR: O diretório de código fonte do pgModeler '$$PGMODELER_SRC_DIR' não foi encontrado! Certifique-se de que a variável PGMODELER_SRC_DIR aponta para uma localização válida!")
 warning("en_US: The pgModeler source code directory '$$PGMODELER_SRC_DIR' could not be found! Make sure the variable PGMODELER_SRC_DIR points to a valid location!")
 error("qmake aborted.")
}

include($$PGMODELER_SRC_DIR/pgmodeler.pro)

CONFIG += plugin qt warn_on uitools uic4
QT += core gui #qt3support
TEMPLATE = lib
TARGET = dummyplugin
TRANSLATIONS += $$PWD/lang/$$TARGET.en_US.ts

CODECFORTR = UTF8

#unix: CONFIG += link_pkgconfig
#unix: PKGCONFIG += libxml-2.0 libpq

DEPENDPATH = ". res src ui moc obj"
MOC_DIR = moc
OBJECTS_DIR = obj
UI_DIR = src
DESTDIR = build

LIBS += $$PGMODELER_LIB_DIR/$$LIBUTIL \
       $$PGMODELER_LIB_DIR/$$LIBPARSERS \
       $$PGMODELER_LIB_DIR/$$LIBDBCONNECT \
       $$PGMODELER_LIB_DIR/$$LIBPGMODELER \
       $$PGMODELER_LIB_DIR/$$LIBOBJRENDERER \
       $$PGMODELER_LIB_DIR/$$LIBPGMODELERUI

HEADERS += src/dummyplugin.h
SOURCES += src/dummyplugin.cpp

dummy.files = res/dummyplugin.png lang
unix:dummy.files += build/libdummyplugin.so
macx:dummy.files += build/libdummyplugin.dylib
windows:dummy.files += build/dummyplugin.dll build/libdummyplugin.a
dummy.path = $$PGMODELER_PLUGIN_DIR/$$TARGET
INSTALLS += dummy
