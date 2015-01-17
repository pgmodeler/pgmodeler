#Unix or Windows directory configuration
PGMODELER_SRC_DIR=../../

!exists($$PGMODELER_SRC_DIR) {
 warning("The pgModeler source code directory '$$PGMODELER_SRC_DIR' could not be found! Make sure the variable PGMODELER_SRC_DIR points to a valid location!")
 error("qmake aborted.")
}

include(../plugins.pro)

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

HEADERS += src/dummy.h
SOURCES += src/dummy.cpp

windows: DESTDIR += $$PWD

unix|windows: LIBS += -L$$OUT_PWD/../../libpgmodeler_ui/ -lpgmodeler_ui \
                    -L$$OUT_PWD/../../libobjrenderer/ -lobjrenderer \
                    -L$$OUT_PWD/../../libpgconnector/ -lpgconnector \
                    -L$$OUT_PWD/../../libpgmodeler/ -lpgmodeler \
                    -L$$OUT_PWD/../../libparsers/ -lparsers \
                    -L$$OUT_PWD/../../libutils/ -lutils

INCLUDEPATH += $$PWD/../../libpgmodeler_ui \
               $$PWD/../../libpgmodeler_ui/src \
               $$PWD/../../libobjrenderer/src \
               $$PWD/../../libpgconnector/src \
               $$PWD/../../libpgmodeler/src \
               $$PWD/../../libparsers/src \
               $$PWD/../../libutils/src
DEPENDPATH += $$PWD/../../libpgmodeler_ui \
              $$PWD/../../libobjrenderer \
              $$PWD/../../libpgconnector \
              $$PWD/../../libpgmodeler \
              $$PWD/../../libparsers \
              $$PWD/../../libutils

target.path = $$PLUGINSDIR/$$TARGET

resources.path = $$PLUGINSDIR/$$TARGET
resources.files += res/dummy.png lang dummy.json

INSTALLS += target resources
