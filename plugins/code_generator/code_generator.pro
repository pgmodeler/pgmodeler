# code_generator.pro
#

# Unix or Windows directory configuration
PGMODELER_SRC_DIR=../../

!exists($$PGMODELER_SRC_DIR) {
 warning("The pgModeler source code directory '$$PGMODELER_SRC_DIR' could not be found! Make sure the variable PGMODELER_SRC_DIR points to a valid location!")
 error("qmake aborted.")
}

include(../plugins.pro)

CONFIG += plugin qt uic4
QT += core gui uitools
TEMPLATE = lib
TARGET = code_generator
OTHER_FILES += code_generator.json
CODECFORTR = UTF8
DEPENDPATH = ". res src ui moc obj"
MOC_DIR = moc
OBJECTS_DIR = obj
UI_DIR = src

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

HEADERS += src/code_generator_plugin.h \
           src/code_generator_widget.h \
           src/base_code_generator.h \
           src/python_daos_generator.h \
           src/base_logger.h

SOURCES += src/code_generator_plugin.cpp \
           src/code_generator_widget.cpp \
           src/python_daos_generator.cpp

FORMS += ui/code_generator_widget.ui

target.path = $$PLUGINSDIR/$$TARGET

resources.path = $$PLUGINSDIR/$$TARGET
resources.files += res/code_generator.png lang code_generator.json

INSTALLS += target resources
