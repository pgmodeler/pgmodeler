include(../../pgmodeler.pri)

TEMPLATE = app
TARGET = pgmodeler-sc

windows:RC_FILE=res/windows_ico.qrc
windows:RCC_DIR=src/

SOURCES += src/main.cpp \
	   src/syntaxcheckerform.cpp \
	   ../pgmodeler/src/pgmodelerapp.cpp

FORMS += ui/syntaxcheckerform.ui

HEADERS += src/syntaxcheckerform.h

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
	       ../pgmodeler/src

DEPENDPATH += $$LIBGUI_ROOT \
	      $$LIBCANVAS_ROOT \
	      $$LIBCONNECTOR_ROOT \
	      $$LIBCORE_ROOT \
	      $$LIBPARSERS_ROOT \
	      $$LIBUTILS_ROOT \
	      ../pgmodeler

# Deployment settings
target.path = $$PRIVATEBINDIR
INSTALLS = target

# Print the current build settins (see pgmodeler.pri)
printBuildDetails()
