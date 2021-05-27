include(../pgmodeler.pri)

TEMPLATE = app
QT += testlib

unix|windows: LIBS += $$PGMUI_LIB \
		      $$PGMCANVAS_LIB \
		      $$PGMCONNECTOR_LIB \
		      $$PGMCORE_LIB \
		      $$PGMPARSERS_LIB \
		      $$PGMUTILS_LIB

INCLUDEPATH += $$PGMUI_INC \
	       $$PGMCANVAS_INC \
	       $$PGMCONNECTOR_INC \
	       $$PGMCORE_INC \
	       $$PGMPARSERS_INC \
	       $$PGMUTILS_INC \
	       $$PWD/src

DEPENDPATH += $$PGMUI_ROOT \
	      $$PGMCANVAS_ROOT \
	      $$PGMCONNECTOR_ROOT \
	      $$PGMCORE_ROOT \
	      $$PGMPARSERS_ROOT \
	      $$PGMUTILS_ROOT \
	      $$PWD/src

#unix|win32: LIBS += -L$$OUT_PWD/../../../libs/pgmui/ -lpgmui \
#		    -L$$OUT_PWD/../../../libs/pgmcanvas/ -lpgmcanvas \
#		    -L$$OUT_PWD/../../../libs/pgmconnector/ -lpgmconnector \
#		    -L$$OUT_PWD/../../../libs/pgmcore/ -lpgmcore \
#		    -L$$OUT_PWD/../../../libs/pgmparsers/ -lpgmparsers \
#		    -L$$OUT_PWD/../../../libs/pgmutils/ -lpgmutils

#INCLUDEPATH += $$PWD/../libs/pgmui/src \
#	       $$PWD/../libs/pgmcanvas/src \
#	       $$PWD/../libs/pgmconnector/src \
#	       $$PWD/../libs/pgmcore/src \
#	       $$PWD/../libs/pgmparsers/src \
#	       $$PWD/../libs/pgmutils/src \
#	       $$PWD/../apps/gui/src \
#               $$PWD/src

#DEPENDPATH += $$PWD/../libs/pgmui \
#	      $$PWD/../libs/pgmcanvas \
#	      $$PWD/../libs/pgmconnector \
#	      $$PWD/../libs/pgmcore \
#	      $$PWD/../libs/pgmparsers \
#	      $$PWD/../libs/pgmutils \
#	      $$PWD/../apps/gui \
#              $$PWD/src

HEADERS += $$PWD/src/pgmodelerunittest.h

# Deployment settings
target.path = $$BINDIR/tests
INSTALLS = target
