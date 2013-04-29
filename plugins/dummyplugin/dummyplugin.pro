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
TARGET = dummyplugin
TRANSLATIONS += $$PWD/lang/$$TARGET.en_US.ts

CODECFORTR = UTF8

DEPENDPATH = ". res src ui moc obj"
MOC_DIR = moc
OBJECTS_DIR = obj
UI_DIR = src
DESTDIR = build

QMAKE_LFLAGS += -L$$DESTDIR

HEADERS += src/dummyplugin.h
SOURCES += src/dummyplugin.cpp

dummy.files = res/dummyplugin.png lang
unix:dummy.files += build/libdummyplugin.so
macx:dummy.files += build/libdummyplugin.dylib
windows:dummy.files += build/dummyplugin.dll build/libdummyplugin.a
dummy.path = $$PGMODELER_PLUGIN_DIR/$$TARGET
INSTALLS += dummy
