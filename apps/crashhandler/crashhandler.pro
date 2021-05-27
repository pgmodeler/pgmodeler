include(../../pgmodeler.pri)

TEMPLATE = app
TARGET = pgmodeler-ch

windows:RC_FILE=res/windows_ico.qrc
windows:RCC_DIR=src/

SOURCES += src/main.cpp \
           src/crashhandlerform.cpp

HEADERS += src/crashhandlerform.h

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
	       $$PGMUTILS_INC

DEPENDPATH += $$PGMUI_ROOT \
	      $$PGMCANVAS_ROOT \
	      $$PGMCONNECTOR_ROOT \
	      $$PGMCORE_ROOT \
	      $$PGMPARSERS_ROOT \
	      $$PGMUTILS_ROOT

# Deployment settings
target.path = $$PRIVATEBINDIR
INSTALLS = target
