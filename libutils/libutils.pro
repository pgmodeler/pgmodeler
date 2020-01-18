# libutils.pro (reviewed version)
#
# Refactored by: Lisandro Damián Nicanor Pérez Meyer <perezmeyer@gmail.com>
# Refactored code: https://github.com/perezmeyer/pgmodeler/tree/shared_libs
# Reviewed by: Raphal Araújo e Silva <raphael@pgmodeler.com.br>
#
# NOTE: Reviewed code is not a direct merge from refactored version but based upon the
# refactored code, containing almost all changes done by the refactoring author.

include(../pgmodeler.pri)

TEMPLATE = lib
TARGET = utils
windows: DESTDIR = $$PWD

HEADERS += src/exception.h \
           src/globalattributes.h \
           src/pgsqlversions.h \
	   src/doublenan.h \
	   src/application.h

SOURCES += src/exception.cpp \
           src/globalattributes.cpp \
	   src/pgsqlversions.cpp \
	   src/application.cpp

# Deployment settings
target.path = $$PRIVATELIBDIR
INSTALLS = target
