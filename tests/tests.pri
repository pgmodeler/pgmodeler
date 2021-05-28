include(../pgmodeler.pri)

TEMPLATE = app
QT += testlib

unix|windows: LIBS += $$LIBGUI_LIB \
		      $$LIBCANVAS_LIB \
		      $$LIBCONNECTOR_LIB \
		      $$LIBCORE_LIB \
		      $$LIBPARSERS_LIB \
		      $$LIBUTILS_LIB

INCLUDEPATH += $$LIBGUI_INC \
	       $$LIBCANVAS_INC \
	       $$LIBCONNECTOR_INC \
	       $$LIBCORE_INC \
	       $$LIBPARSERS_INC \
	       $$LIBUTILS_INC \
	       $$PWD/src

DEPENDPATH += $$LIBGUI_ROOT \
	      $$LIBCANVAS_ROOT \
	      $$LIBCONNECTOR_ROOT \
	      $$LIBCORE_ROOT \
	      $$LIBPARSERS_ROOT \
	      $$LIBUTILS_ROOT \
	      $$PWD/src

HEADERS += $$PWD/src/pgmodelerunittest.h

# Deployment settings
target.path = $$BINDIR/tests
INSTALLS = target
