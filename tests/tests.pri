# tests.pro (reviewed version)
#
# Refactored by: Lisandro Damián Nicanor Pérez Meyer <perezmeyer@gmail.com>
# Refactored code: https://github.com/perezmeyer/pgmodeler/tree/shared_libs
# Reviewed by: Raphal Araújo e Silva <raphael@pgmodeler.com.br>
#
# NOTE: Reviewed code is not a direct merge from refactored version but based upon the
# refactored code, containing almost all changes done by the refactoring author.

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
               $$PWD/../libutils/src

DEPENDPATH += $$PWD/../libpgmodeler_ui \
              $$PWD/../libobjrenderer \
              $$PWD/../libpgconnector \
              $$PWD/../libpgmodeler \
              $$PWD/../libparsers \
              $$PWD/../libutils

# Deployment settings
target.path = $$BINDIR/tests
INSTALLS = target
