# This file contains the main variables settings to build pgModeler on all supported platforms.
#
# Thanks to Lisandro Damián Nicanor Pérez Meyer, pgModeler is able to be package in most of
# Linux distros.
#
# Original version by: Lisandro Damián Nicanor Pérez Meyer <perezmeyer@gmail.com>
# Original code: https://github.com/perezmeyer/pgmodeler/tree/shared_libs
#
# Refactored version by: Raphal Araújo e Silva <raphael@pgmodeler.com.br>
# Refactored code: https://github.com/pgmodeler/pgmodeler

# General Qt settings
QT += core widgets printsupport network svg
CONFIG += ordered qt stl rtti exceptions warn_on c++14
TEMPLATE = subdirs
MOC_DIR = moc
OBJECTS_DIR = obj
UI_DIR = src

# Setting up the flag passed to compiler to indicate a snapshot build
defined(SNAPSHOT_BUILD, var): DEFINES+=SNAPSHOT_BUILD

# Setting up the flag passed to compiler to build the demo version
defined(DEMO_VERSION, var): DEFINES+=DEMO_VERSION

# Setting up the flag passed to compiler to disable all code related to update checking
defined(NO_UPDATE_CHECK, var): DEFINES+=NO_UPDATE_CHECK

# Properly defining build number constant
unix {
 BUILDNUM=$$system("date '+%Y%m%d'")
 DEFINES+=BUILDNUM=\\\"$${BUILDNUM}\\\"
} else {
 BUILDNUM=$$system('wingetdate.bat')
 DEFINES+=BUILDNUM=\\\"$${BUILDNUM}\\\"
}

# Below, the user can specify where all generated file can be placed
# through a set of variables, being them:
#
# PREFIX        -> the root directory where the files will be placed
# BINDIR        -> where executables accessible by the user resides
# PRIVATEBINDIR -> where executables not directly accessible by the user resides
# PRIVATELIBDIR -> where libraries not directly shared through the system resides
# PLUGINSDIR    -> where third party plugins are installed
# SHAREDIR      -> where shared files and resources should be placed
# CONFDIR       -> where the pgModeler's configuration folder (conf) resides
# DOCDIR        -> where documentation related files are placed
# LANGDIR       -> where the UI translation folder (lang) resides
# SAMPLESDIR    -> where the sample models folder (samples) resides
# SCHEMASDIR    -> where the object's schemas folder (schema) resides
#
# The values of each variable changes between supported platforms and are describe as follow


# Linux custom variables settings
linux {
  CONFIG += x11

  # If the AppImage generation option is set
  defined(APPIMAGE_BUILD, var):{
	!defined(PREFIX, var): PREFIX = /usr/local/pgmodeler-appimage
	BINDIR = $$PREFIX
	PRIVATEBINDIR = $$PREFIX
	PRIVATELIBDIR = $$PREFIX
	PLUGINSDIR = $$PREFIX/lib/pgmodeler/plugins
	SHAREDIR = $$PREFIX
	CONFDIR = $$SHAREDIR/conf
	DOCDIR = $$SHAREDIR
	LANGDIR = $$SHAREDIR/lang
	SAMPLESDIR = $$SHAREDIR/samples
	SCHEMASDIR = $$SHAREDIR/schemas
  }

  !defined(APPIMAGE_BUILD, var):{
	# Default configuration for package pgModeler.
	# The default prefix is /usr/local
	!defined(PREFIX, var):        PREFIX = /usr/local
	!defined(BINDIR, var):        BINDIR = $$PREFIX/bin
	!defined(PRIVATEBINDIR, var): PRIVATEBINDIR = $$PREFIX/bin
	!defined(PRIVATELIBDIR, var): PRIVATELIBDIR = $$PREFIX/lib/pgmodeler
	!defined(PLUGINSDIR, var):    PLUGINSDIR = $$PREFIX/lib/pgmodeler/plugins
	!defined(SHAREDIR, var):      SHAREDIR = $$PREFIX/share/pgmodeler
	!defined(CONFDIR, var):       CONFDIR = $$SHAREDIR/conf
	!defined(DOCDIR, var):        DOCDIR = $$SHAREDIR
	!defined(LANGDIR, var):       LANGDIR = $$SHAREDIR/lang
	!defined(SAMPLESDIR, var):    SAMPLESDIR = $$SHAREDIR/samples
	!defined(SCHEMASDIR, var):    SCHEMASDIR = $$SHAREDIR/schemas
 }

  # Specifies where to find the libraries at runtime
  RELATIVE_PRIVATELIBDIR = $$relative_path($$PRIVATELIBDIR, $$BINDIR)
  QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN\' -Wl,-rpath,\'\$$ORIGIN/$$RELATIVE_PRIVATELIBDIR\'"

  # Forcing the display of some warnings
  CONFIG(debug, debug|release): QMAKE_CXXFLAGS += "-Wall -Wextra -Wuninitialized"
}


# Windows custom variables settings
windows {
  CONFIG += windows

  # The default prefix is ./build
  !defined(PREFIX, var):        PREFIX = $$PWD/build
  !defined(BINDIR, var):        BINDIR = $$PREFIX
  !defined(PRIVATEBINDIR, var): PRIVATEBINDIR = $$PREFIX
  !defined(PRIVATELIBDIR, var): PRIVATELIBDIR = $$PREFIX
  !defined(PLUGINSDIR, var):    PLUGINSDIR = $$PREFIX/plugins
  !defined(SHAREDIR, var):      SHAREDIR = $$PREFIX
  !defined(CONFDIR, var):       CONFDIR = $$PREFIX/conf
  !defined(DOCDIR, var):        DOCDIR = $$PREFIX
  !defined(LANGDIR, var):       LANGDIR = $$PREFIX/lang
  !defined(SAMPLESDIR, var):    SAMPLESDIR = $$PREFIX/samples
  !defined(SCHEMASDIR, var):    SCHEMASDIR = $$PREFIX/schemas
}


# MacOS X custom variables settings
macx {
  CONFIG -= app_bundle

  # The default prefix is ./build/pgmodeler.app/Contents
  !defined(PREFIX, var):        PREFIX = /Applications/pgModeler.app/Contents
  !defined(BINDIR, var):        BINDIR = $$PREFIX/MacOS
  !defined(PRIVATEBINDIR, var): PRIVATEBINDIR = $$BINDIR
  !defined(PRIVATELIBDIR, var): PRIVATELIBDIR = $$PREFIX/Frameworks
  !defined(PLUGINSDIR, var):    PLUGINSDIR = $$BINDIR/plugins
  !defined(SHAREDIR, var):      SHAREDIR = $$BINDIR
  !defined(CONFDIR, var):       CONFDIR = $$BINDIR/conf
  !defined(DOCDIR, var):        DOCDIR = $$BINDIR
  !defined(LANGDIR, var):       LANGDIR = $$BINDIR/lang
  !defined(SAMPLESDIR, var):    SAMPLESDIR = $$BINDIR/samples
  !defined(SCHEMASDIR, var):    SCHEMASDIR = $$BINDIR/schemas

  # Specifies where to find the libraries at runtime
  QMAKE_RPATHDIR += @executable_path/../Frameworks
  QMAKE_LFLAGS_SONAME = -Wl,-install_name,@loader_path/../Frameworks/
}

# Creating constants based upon the custom paths so the GlobalAttributes
# namespace can correctly configure the paths inside the code
DEFINES += BINDIR=\\\"$${BINDIR}\\\" \
           PLUGINSDIR=\\\"$${PLUGINSDIR}\\\" \
           PRIVATEBINDIR=\\\"$${PRIVATEBINDIR}\\\" \
           CONFDIR=\\\"$${CONFDIR}\\\" \
           DOCDIR=\\\"$${DOCDIR}\\\" \
           LANGDIR=\\\"$${LANGDIR}\\\" \
           SAMPLESDIR=\\\"$${SAMPLESDIR}\\\" \
           SCHEMASDIR=\\\"$${SCHEMASDIR}\\\"


# pgModeler depends on libpq and libxml2 this way to variables
# are define so the compiler can find the libs at link time.
#
# PGSQL_LIB -> Full path to libpq.(so | dll | dylib)
# PGSQL_INC -> Root path where PgSQL includes can be found
#
# XML_LIB   -> Full path to libxml2.(so | dll | dylib)
# XML_INC   -> Root path where XML2 includes can be found

unix:!macx {
  CONFIG += link_pkgconfig
  PKGCONFIG = libpq libxml-2.0
  PGSQL_LIB = -lpq
  XML_LIB = -lxml2
}

macx {
  PGSQL_LIB = /Library/PostgreSQL/11/lib/libpq.dylib
  PGSQL_INC = /Library/PostgreSQL/11/include
  XML_INC = /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/libxml2
  XML_LIB = /usr/lib/libxml2.dylib
  INCLUDEPATH += $$PGSQL_INC $$XML_INC
}

windows {
  !defined(PGSQL_LIB, var): PGSQL_LIB = C:/msys64/mingw64/bin/libpq.dll
  !defined(PGSQL_INC, var): PGSQL_INC = C:/msys64/mingw64/include
  !defined(XML_INC, var): XML_INC = C:/msys64/mingw64/include/libxml2
  !defined(XML_LIB, var): XML_LIB = C:/msys64/mingw64/bin/libxml2-2.dll

  # Workaround to solve bug of timespec struct on MingW + PostgreSQL < 9.4
  QMAKE_CXXFLAGS+="-DHAVE_STRUCT_TIMESPEC"

  INCLUDEPATH += "$$PGSQL_INC" "$$XML_INC"
}

macx | windows {
  !exists($$PGSQL_LIB) {
    PKG_ERROR = "PostgreSQL libraries"
    VARIABLE = "PGSQL_LIB"
    VALUE = $$PGSQL_LIB
  }

  !exists($$PGSQL_INC/libpq-fe.h) {
    PKG_ERROR = "PostgreSQL headers"
    VARIABLE = "PGSQL_INC"
    VALUE = $$PGSQL_INC
  }

  !exists($$XML_LIB) {
    PKG_ERROR = "XML2 libraries"
    VARIABLE = "XML_LIB"
    VALUE = $$XML_LIB
  }

  !exists($$XML_INC) {
    PKG_ERROR = "XML2 headers"
    VARIABLE = "XML_INC"
    VALUE = $$XML_INC
  }

  !isEmpty(PKG_ERROR) {
    warning("$$PKG_ERROR were not found at \"$$VALUE\"!")
    warning("Please correct the value of $$VARIABLE and try again!")
    error("pgModeler compilation aborted.")
  }
}

# Define a custom function to print build details
defineTest(printBuildDetails) {
 LB=$$escape_expand(\n)
 log($$LB)
 log("** pgModeler build details ** $$LB $$LB")
 log("  PREFIX        = $$PREFIX $$LB")
 log("  BINDIR        = $$BINDIR $$LB")
 log("  PRIVATEBINDIR = $$PRIVATEBINDIR $$LB")
 log("  PRIVATELIBDIR = $$PRIVATELIBDIR $$LB")
 log("  PLUGINSDIR    = $$PLUGINSDIR $$LB")
 log("  SHAREDIR      = $$SHAREDIR $$LB")
 log("  CONFDIR       = $$CONFDIR $$LB")
 log("  DOCDIR        = $$DOCDIR $$LB")
 log("  LANGDIR       = $$LANGDIR $$LB")
 log("  SAMPLESDIR    = $$SAMPLESDIR $$LB")
 log("  SCHEMASDIR    = $$SCHEMASDIR $$LB $$LB")
 log("* To change a variable value run qmake again setting the desired value e.g.: $$LB")
 log("  > qmake PREFIX+=/usr/local -r pgmodeler.pro $$LB $$LB")
 log("* Proceed with build process by running: $$LB")
 log("  >  make && make install $$LB")
 log($$LB)
 return(true)
}
