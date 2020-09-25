include(../pgmodeler.pri)

CONFIG += console
TEMPLATE = app
TARGET = pgmodeler-cli
INCLUDEPATH += ../main/src

windows:RC_FILE=../main/res/windows_ico.qrc
windows: RCC_DIR=src/
windows: DESTDIR = $$PWD

SOURCES += src/main.cpp \
    src/pgmodelercliapp.cpp

HEADERS += \
    src/pgmodelercliapp.h

unix|windows: LIBS += -L$$OUT_PWD/../libpgmodeler_ui/ -lpgmodeler_ui \
                    -L$$OUT_PWD/../libobjrenderer/ -lobjrenderer \
                    -L$$OUT_PWD/../libpgconnector/ -lpgconnector \
                    -L$$OUT_PWD/../libpgmodeler/ -lpgmodeler \
                    -L$$OUT_PWD/../libparsers/ -lparsers \
                    -L$$OUT_PWD/../libutils/ -lutils

INCLUDEPATH += $$PWD/../libpgmodeler_ui/src \
               $$PWD/../libobjrenderer/src \
               $$PWD/../libpgconnector/src \
               $$PWD/../libpgmodeler/src \
               $$PWD/../libparsers/src \
               $$PWD/../libutils/src

DEPENDPATH += $$PWD/../libpgmodeler_ui \
              $$PWD/../libobjrenderer \
              $$PWD/../libpgconnector \
              $$PWD/../libpgmodeler \
              $$PWD/../libparsers \
              $$PWD/../libutils

# Deployment settings
target.path = $$BINDIR
INSTALLS = target
