include(../../pgmodeler.pri)

TEMPLATE = app
TARGET = pgmodeler-se

windows:RC_FILE=res/windows_ico.qrc
windows:RCC_DIR=src/

SOURCES += src/main.cpp \
	   src/aboutwidget.cpp \
	src/schemaeditorform.cpp \
	   src/sourceeditorwidget.cpp \
	   ../pgmodeler/src/pgmodelerapp.cpp \

FORMS += ui/syntaxcheckerform.ui \
	 ui/aboutwidget.ui \
	 ui/sourceeditorwidget.ui

HEADERS += \
	   src/aboutwidget.h \
	   src/schemaeditorform.h \
	   src/sourceeditorwidget.h

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
