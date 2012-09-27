PGMODELER_SRC_DIR=../../
PGMODELER_LIB_DIR=$$PGMODELER_SRC_DIR/build/lib

!exists($$PGMODELER_SRC_DIR) {
 warning("pt_BR: O diretório de código fonte do pgModeler '$$PGMODELER_SRC_DIR' não foi encontrado! Certifique-se de que a variável PGMODELER_SRC_DIR aponta para uma localização válida!")
 warning("en_US: The pgModeler source code directory '$$PGMODELER_SRC_DIR' could not be found! Make sure the variable PGMODELER_SRC_DIR points to a valid location!")
 error("qmake aborted.")
}

include($$PGMODELER_SRC_DIR/pgmodeler.pro)

CONFIG += plugin qt warn_on uitools uic4
QT = core gui qt3support
TEMPLATE = lib
TARGET = dummyplugin
TRANSLATIONS += $$PWD/lang/$$TARGET.en_US.ts

CODECFORTR = UTF8

INCLUDEPATH = $${GLOBAL_INCLUDES} \
              $$PGMODELER_SRC_DIR/libutil/src \
              $$PGMODELER_SRC_DIR/libconexbd/src \
              $$PGMODELER_SRC_DIR/libparsers/src \
              $$PGMODELER_SRC_DIR/libpgmodeler/src \
              $$PGMODELER_SRC_DIR/libobjrenderer/src \
              $$PGMODELER_SRC_DIR/libpgmodeler_ui/src

DEPENDPATH = ". res src ui moc obj"
MOC_DIR = moc
OBJECTS_DIR = obj
UI_DIR = src
DESTDIR = ../../build/plugins/$$TARGET

LIBS = $$PGMODELER_LIB_DIR/$$LIBUTIL \
       $$PGMODELER_LIB_DIR/$$LIBPARSERS \
       $$PGMODELER_LIB_DIR/$$LIBCONEXBD \
       $$PGMODELER_LIB_DIR/$$LIBOBJRENDERER \
       $$PGMODELER_LIB_DIR/$$LIBPGMODELER \
       $$PGMODELER_LIB_DIR/$$LIBPGMODELERUI

unix {
 QMAKE_POST_LINK+= "cp res/$$TARGET.png $$DESTDIR;\
                    cp -r lang/ $$DESTDIR;"
 QMAKE_DISTCLEAN+= "-r $$MOC_DIR $$OBJECTS_DIR"
}

windows {
 DEST=..\\..\\build\\plugins\\$$TARGET
 QMAKE_POST_LINK+= "copy res\\$$TARGET.png $$DEST /Y &\
                    xcopy lang $$DEST\\lang /S /I /Y /Q"
 QMAKE_DISTCLEAN+= "$$MOC_DIR /S /Q & \
                    $$OBJECTS_DIR  /S /Q"
}


HEADERS += src/dummyplugin.h
SOURCES += src/dummyplugin.cpp

