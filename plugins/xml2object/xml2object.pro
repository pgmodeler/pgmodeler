PGMODELER_SRC_DIR=../../
PGMODELER_LIB_DIR=../../build
PGMODELER_PLUGIN_DIR=$$PGMODELER_SRC_DIR/build/plugins

!exists($$PGMODELER_SRC_DIR) {
 warning("The pgModeler source code directory '$$PGMODELER_SRC_DIR' could not be found! Make sure the variable PGMODELER_SRC_DIR points to a valid location!")
 error("qmake aborted.")
}

include($$PGMODELER_SRC_DIR/pgmodeler.pro)

CONFIG += plugin qt warn_on uic4
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
	$$PGMODELER_LIB_DIR/$$LIBDBCONNECT \
	$$PGMODELER_LIB_DIR/$$LIBOBJRENDERER \
	$$PGMODELER_LIB_DIR/$$LIBPGMODELER \
	$$PGMODELER_LIB_DIR/$$LIBPGMODELERUI

HEADERS += src/xml2object.h \
	   src/xml2objectwidget.h

SOURCES += src/xml2object.cpp \
	   src/xml2objectwidget.cpp

FORMS += ui/xml2objectwidget.ui

unix:QMAKE_POST_LINK="chmod 644 $$DESTDIR/$${LIB_PREFIX}$${TARGET}.$${LIB_EXT}"

xml2object.files += res/xml2object.png lang xml2object.json $$DESTDIR/$${LIB_PREFIX}$${TARGET}.$${LIB_EXT}
windows:xml2object.files += build/$$LIB_PREFIX$$TARGET.a
xml2object.path = $$PGMODELER_PLUGIN_DIR/$$TARGET
INSTALLS += xml2object
