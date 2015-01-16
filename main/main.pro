include(../pgmodeler.pri)

TEMPLATE = app
TARGET = pgmodeler

windows:RC_FILE=res/windows_ico.qrc
windows:RCC_DIR=src/

macx:QMAKE_POST_LINK+="cp -r $$PWD/res/Resources $$PREFIX; \
                       cp $$PWD/res/Info.plist $$PREFIX; \
                       cp $$PWD/res/PkgInfo $$PREFIX; \
                       cp $$PWD/res/startapp $$BINDIR"

HEADERS += src/application.h
SOURCES += src/main.cpp \
           src/application.cpp

unix|win32: LIBS += -L$$OUT_PWD/../libpgmodeler_ui/ -lpgmodeler_ui \
                    -L$$OUT_PWD/../libobjrenderer/ -lobjrenderer \
                    -L$$OUT_PWD/../libpgconnector/ -lpgconnector \
                    -L$$OUT_PWD/../libpgmodeler/ -lpgmodeler \
                    -L$$OUT_PWD/../libparsers/ -lparsers \
                    -L$$OUT_PWD/../libutils/ -lutils

INCLUDEPATH += $$PWD/../libpgmodeler_ui \
               $$PWD/../libpgmodeler_ui/src \
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


