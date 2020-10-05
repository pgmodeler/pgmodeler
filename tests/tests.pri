include(../pgmodeler.pri)

TEMPLATE = app
QT += testlib

unix|win32: LIBS += -L$$OUT_PWD/../../../libpgmodeler_ui/ -lpgmodeler_ui \
                    -L$$OUT_PWD/../../../libobjrenderer/ -lobjrenderer \
                    -L$$OUT_PWD/../../../libpgconnector/ -lpgconnector \
                    -L$$OUT_PWD/../../../libpgmodeler/ -lpgmodeler \
                    -L$$OUT_PWD/../../../libparsers/ -lparsers \
                    -L$$OUT_PWD/../../../libutils/ -lutils

INCLUDEPATH += $$PWD/../libpgmodeler_ui/src \
               $$PWD/../libobjrenderer/src \
               $$PWD/../libpgconnector/src \
               $$PWD/../libpgmodeler/src \
               $$PWD/../libparsers/src \
               $$PWD/../libutils/src \
               $$PWD/../main/src \
               $$PWD/src

DEPENDPATH += $$PWD/../libpgmodeler_ui \
              $$PWD/../libobjrenderer \
              $$PWD/../libpgconnector \
              $$PWD/../libpgmodeler \
              $$PWD/../libparsers \
              $$PWD/../libutils \
			  $$PWD/../main \
              $$PWD/src

HEADERS += $$PWD/src/pgmodelerunittest.h

# Deployment settings
target.path = $$BINDIR/tests
INSTALLS = target
