#Unix or Windows directory configuration
PGMODELER_SRC_DIR=../../

!exists($$PGMODELER_SRC_DIR) {
 warning("The pgModeler source code directory '$$PGMODELER_SRC_DIR' could not be found! Make sure the variable PGMODELER_SRC_DIR points to a valid location!")
 error("qmake aborted.")
}

include($$PGMODELER_SRC_DIR/pgmodeler.pro)

#PGMODELER_LIB_DIR=$$LIBDESTDIR
PGMODELER_LIB_DIR=$$PGMODELER_SRC_DIR/build
PGMODELER_PLUGIN_DIR=$$RESDESTDIR/plugins

macx {
 PGMODELER_SRC_DIR=../../../
 PGMODELER_BASE_DIR=$$PGMODELER_SRC_DIR/build/pgmodeler.app/Contents
 PGMODELER_LIB_DIR=$$PGMODELER_BASE_DIR/Frameworks
 PGMODELER_PLUGIN_DIR=$$PGMODELER_BASE_DIR/MacOS/plugins
}

CONFIG += plugin qt uic4
QT += core gui uitools
TEMPLATE = lib
TARGET = xml2object
OTHER_FILES += xml2object.json
CODECFORTR = UTF8
DEPENDPATH = ". res src ui moc obj"
MOC_DIR = moc
OBJECTS_DIR = obj
UI_DIR = src
DESTDIR = build

LIBS += $$PGMODELER_LIB_DIR/$$LIBUTILS \
	$$PGMODELER_LIB_DIR/$$LIBPARSERS \
	$$PGMODELER_LIB_DIR/$$LIBPGCONNECTOR \
	$$PGMODELER_LIB_DIR/$$LIBOBJRENDERER \
	$$PGMODELER_LIB_DIR/$$LIBPGMODELER \
	$$PGMODELER_LIB_DIR/$$LIBPGMODELERUI

HEADERS += src/xml2object.h \
	   src/xml2objectwidget.h

SOURCES += src/xml2object.cpp \
	   src/xml2objectwidget.cpp

FORMS += ui/xml2objectwidget.ui

unix:QMAKE_POST_LINK="chmod 644  $$DESTDIR/$${LIB_PREFIX}$${TARGET}.$${LIB_EXT}"
windows:QMAKE_POST_LINK="cacls $$DESTDIR/$${LIB_PREFIX}$${TARGET}.$${LIB_EXT} /E /P :R"

xml2object.path = $$PGMODELER_PLUGIN_DIR/$$TARGET
xml2object.files += res/xml2object.png lang xml2object.json $$DESTDIR/$${LIB_PREFIX}$${TARGET}.$${LIB_EXT}
windows:xml2object.files += $$DESTDIR/$$LIB_PREFIX$$TARGET.a

# This line force the copy of the generated .so to the destination folder.
unix:xml2object.extra = cp $$DESTDIR/$${LIB_PREFIX}$${TARGET}.$${LIB_EXT} $$PGMODELER_PLUGIN_DIR/$$TARGET > /dev/null

INSTALLS += xml2object
