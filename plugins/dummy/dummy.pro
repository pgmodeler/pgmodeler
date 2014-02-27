#Unix or Windows directory configuration
PGMODELER_SRC_DIR=../../

!exists($$PGMODELER_SRC_DIR) {
 warning("The pgModeler source code directory '$$PGMODELER_SRC_DIR' could not be found! Make sure the variable PGMODELER_SRC_DIR points to a valid location!")
 error("qmake aborted.")
}

include($$PGMODELER_SRC_DIR/pgmodeler.pro)

PGMODELER_LIB_DIR=$$DESTDIR
PGMODELER_PLUGIN_DIR=$$LIBDESTDIR/plugins

macx {
 PGMODELER_SRC_DIR=../../
 PGMODELER_BASE_DIR=../../build/pgmodeler.app/Contents
 PGMODELER_LIB_DIR=$$PGMODELER_BASE_DIR/Frameworks
 PGMODELER_PLUGIN_DIR=$$PGMODELER_BASE_DIR/MacOS/plugins
}

CONFIG += plugin qt uic4
QT += core gui uitools
TEMPLATE = lib
TARGET = dummy
TRANSLATIONS += $$PWD/lang/$$TARGET.en_US.ts
OTHER_FILES += dummy.json
CODECFORTR = UTF8
DEPENDPATH = ". res src ui moc obj"
MOC_DIR = moc
OBJECTS_DIR = obj
UI_DIR = src

LIBS += $$PGMODELER_LIB_DIR/$$LIBUTILS \
	$$PGMODELER_LIB_DIR/$$LIBPARSERS \
	$$PGMODELER_LIB_DIR/$$LIBPGCONNECTOR \
	$$PGMODELER_LIB_DIR/$$LIBOBJRENDERER \
	$$PGMODELER_LIB_DIR/$$LIBPGMODELER \
	$$PGMODELER_LIB_DIR/$$LIBPGMODELERUI

HEADERS += src/dummy.h
SOURCES += src/dummy.cpp

unix:QMAKE_POST_LINK="chmod 644 $$DESTDIR/$${LIB_PREFIX}$${TARGET}.$${LIB_EXT}"
windows:QMAKE_POST_LINK="cacls $$DESTDIR/$${LIB_PREFIX}$${TARGET}.$${LIB_EXT} /E /P :R"

dummy.files += res/dummy.png lang dummy.json
windows:dummy.files += $$DESTDIR/$$LIB_PREFIX$$TARGET.a
dummy.path = $$PGMODELER_PLUGIN_DIR/$$TARGET

target.path = $$PGMODELER_PLUGIN_DIR/$$TARGET
INSTALLS += target dummy
