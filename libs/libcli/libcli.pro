include(../../pgmodeler.pri)

TEMPLATE = lib
TARGET = cli
windows: DESTDIR = $$PWD

# Enables shared library symbols exporting
DEFINES += CLI_SYMBOLS

SOURCES += src/compat/compatns.cpp \
src/pgmodelercliapp.cpp \
src/compat/view.cpp \
src/compat/reference.cpp \
src/pgmodelercliplugin.cpp

HEADERS += src/pgmodelercliapp.h \
src/compat/compatns.h \
src/compat/view.h \
src/compat/reference.h \
src/pgmodelercliplugin.h

INCLUDEPATH += $$LIBCANVAS_INC \
				 $$LIBCONNECTOR_INC \
				 $$LIBCORE_INC \
				 $$LIBPARSERS_INC \
				 $$LIBUTILS_INC \
				 $$LIBGUI_INC \
				 $$PWD/src/compat

DEPENDPATH += $$LIBCANVAS_ROOT \
				$$LIBCONNECTOR_ROOT \
				$$LIBCORE_ROOT \
				$$LIBPARSERS_ROOT \
				$$LIBUTILS_ROOT \
				$$LIBGUI_ROOT

# Deployment settings
target.path = $$PRIVATELIBDIR
INSTALLS = target
