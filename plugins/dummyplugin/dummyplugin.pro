PGMODELER_SRC_DIR=/root/pgmodeler

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
TRANSLATIONS = $$PWD/lang/dummyplugin.en_US.ts

CODECFORTR = UTF-8

INCLUDEPATH = $${GLOBAL_INCLUDES} \
              $$PGMODELER_SRC_DIR/libutil/src \
              $$PGMODELER_SRC_DIR/libconexbd/src \
              $$PGMODELER_SRC_DIR/libparsers/src \
              $$PGMODELER_SRC_DIR/libpgmodeler/src \
              $$PGMODELER_SRC_DIR/libobjrenderer/src \
              $$PGMODELER_SRC_DIR/pgmodeler_ui/src

DEPENDPATH = ". res src ui moc obj"
MOC_DIR = moc
OBJECTS_DIR = obj
UI_DIR = src
DESTDIR = ../../build/plugins/dummyplugin

QMAKE_DISTCLEAN+= "-r $$MOC_DIR $$OBJECTS_DIR"

QMAKE_POST_LINK+= "cp res/dummyplugin.png $$DESTDIR;\
                   cp -r lang/ $$DESTDIR;"

HEADERS += src/dummyplugin.h \
           $$PGMODELER_SRC_DIR/pgmodeler_ui/src/pgmodelerplugin.h \
           $$PGMODELER_SRC_DIR/pgmodeler_ui/src/caixamensagem.h \
           $$PGMODELER_SRC_DIR/libutil/src/excecao.h

SOURCES += src/dummyplugin.cpp \
           $$PGMODELER_SRC_DIR/pgmodeler_ui/src/caixamensagem.cpp \
           $$PGMODELER_SRC_DIR/libutil/src/excecao.cpp

