include(../pgmodeler.pri)

TEMPLATE = app
TARGET = pgmodeler

windows:RC_FILE=res/windows_ico.qrc
windows:RCC_DIR=src/
macx:QMAKE_POST_LINK+="cp -r $$PWD/res/Resources $$BASEDIR; \
                       cp $$PWD/res/Info.plist $$BASEDIR; \
                       cp $$PWD/res/PkgInfo $$BASEDIR; \
                       cp $$PWD/res/startapp $$DESTDIR"

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

# Installation
target.path = $$BINDIR
INSTALLS = target


