include(../pgmodeler.pri)

TEMPLATE = app
TARGET = pgmodeler-ch

windows:RC_FILE=res/windows_ico.qrc
windows:RCC_DIR=src/

SOURCES += src/main.cpp \
           src/crashhandlerform.cpp

HEADERS += src/crashhandlerform.h

unix|windows: LIBS += -L$$OUT_PWD/../libpgmodeler_ui/ -lpgmodeler_ui \
                      -L$$OUT_PWD/../libobjrenderer/ -lobjrenderer \
                      -L$$OUT_PWD/../libpgconnector/ -lpgconnector \
                      -L$$OUT_PWD/../libpgmodeler/ -lpgmodeler \
                      -L$$OUT_PWD/../libparsers/ -lparsers \
                      -L$$OUT_PWD/../libutils/ -lutils

INCLUDEPATH += $$PWD/../libpgmodeler_ui/src \
               $$PWD/../libpgmodeler_ui \
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
target.path = $$PRIVATEBINDIR
INSTALLS = target
