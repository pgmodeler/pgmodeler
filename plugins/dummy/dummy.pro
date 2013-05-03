PGMODELER_SRC_DIR=../../
PGMODELER_LIB_DIR=../../build
PGMODELER_PLUGIN_DIR=$$PGMODELER_SRC_DIR/build/plugins

!exists($$PGMODELER_SRC_DIR) {
 warning("The pgModeler source code directory '$$PGMODELER_SRC_DIR' could not be found! Make sure the variable PGMODELER_SRC_DIR points to a valid location!")
 error("qmake aborted.")
}

include($$PGMODELER_SRC_DIR/pgmodeler.pro)

CONFIG += plugin qt warn_on uitools uic4
QT += core gui
TEMPLATE = lib
TARGET = dummy
TRANSLATIONS += $$PWD/lang/$$TARGET.en_US.ts

CODECFORTR = UTF8

DEPENDPATH = ". res src ui moc obj"
MOC_DIR = moc
OBJECTS_DIR = obj
UI_DIR = src
DESTDIR = build

LIBS += $$PGMODELER_LIB_DIR/$$LIBUTILS \
	$$PGMODELER_LIB_DIR/$$LIBPARSERS \
	$$PGMODELER_LIB_DIR/$$LIBDBCONNECT \
	$$PGMODELER_LIB_DIR/$$LIBOBJRENDERER \
	$$PGMODELER_LIB_DIR/$$LIBPGMODELER \
	$$PGMODELER_LIB_DIR/$$LIBPGMODELERUI

HEADERS += src/dummy.h
SOURCES += src/dummy.cpp

dummy.files = res/dummy.png lang
unix:dummy.files += build/libdummy.so
macx:dummy.files += build/libdummy.dylib
windows:dummy.files += build/dummy.dll build/libdummy.a
dummy.path = $$PGMODELER_PLUGIN_DIR/$$TARGET
INSTALLS += dummy
