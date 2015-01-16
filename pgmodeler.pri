# General Qt settings
QT += core widgets printsupport network
CONFIG += ordered qt stl rtti exceptions warn_on c++11
TEMPLATE = subdirs

# Setting up the flag passed to compiler to build the demo version
defined(DEMO_VERSION, var): QMAKE_CXXFLAGS+="-DDEMO_VERSION"


# Below, the user can specify where all generated file can be placed
# through a set of variables, being them:
#
# PREFIX        -> the root directory where the files will be placed
# PRIVATEBINDIR -> where executables not directly accessable to the user resides
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

linux {
  CONFIG += x11

   # The default prefix is ./build
  !defined(PREFIX, var): PREFIX = $$PWD/build

  BINDIR = $$PREFIX
  PRIVATEBINDIR = $$PREFIX
  PRIVATELIBDIR = $$PREFIX/lib
  PLUGINSDIR = $$PREFIX/plugins
  SHAREDIR = $$PREFIX
  CONFDIR = $$PREFIX/conf
  DOCDIR = $$PREFIX
  LANGDIR = $$PREFIX/lang
  SAMPLESDIR = $$PREFIX/samples
  SCHEMASDIR = $$PREFIX/schemas

  # Specifies where to find the libraries at runtime
  QMAKE_RPATHDIR += $$PRIVATELIBDIR
}

windows {
  CONFIG += windows

  # The default prefix is ./build
  !defined(PREFIX, var): $$PWD/build

  BINDIR = $$PREFIX
  PRIVATEBINDIR = $$PREFIX
  PRIVATELIBDIR = $$PREFIX
  PLUGINSDIR = $$PREFIX/plugins
  SHAREDIR = $$PREFIX
  CONFDIR = $$PREFIX/conf
  DOCDIR = $$PREFIX
  LANGDIR = $$PREFIX/lang
  SAMPLESDIR = $$PREFIX/samples
  SCHEMASDIR = $$PREFIX/schemas
}

macx {
  CONFIG -= app_bundle

  # The default prefix is ./build/pgmodeler.app/Contents
  !defined(PREFIX, var): $$PWD/build/pgmodeler.app/Contents

  BINDIR = $$PREFIX/MacOS
  PRIVATEBINDIR = $$BINDIR
  PRIVATELIBDIR = $$PREFIX/Frameworks
  PLUGINSDIR = $$BINDIR/plugins
  SHAREDIR = $$BINDIR
  CONFDIR = $$BINDIR/conf
  DOCDIR = $$BINDIR
  LANGDIR = $$BINDIR/lang
  SAMPLESDIR = $$BINDIR/samples
  SCHEMASDIR = $$BINDIR/schemas
}

# Creating constants based upon the custom paths so the GlobalAttributes
# namespace can correctly configure the paths inside the code
DEFINES += PLUGINSDIR=\\\"$${PLUGINSDIR}\\\" \
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
  PGSQL_LIB = /Library/PostgreSQL/9.3/lib/libpq.dylib
  PGSQL_INC = /Library/PostgreSQL/9.3/include
  XML_INC = /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk/usr/include/libxml2
  XML_LIB = /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk/usr/lib/libxml2.dylib
}

windows {
  PGSQL_LIB = C:/PostgreSQL/9.3/bin/libpq.dll
  PGSQL_INC = C:/PostgreSQL/9.3/include
  XML_INC = C:/Qt/Qt5.4.0/5.4/mingw491_32/include
  XML_LIB = C:/Qt/Qt5.4.0/5.4/mingw491_32/bin/libxml2-2.dll

  # Workaround to solve bug of timespec struct on MingW + PostgreSQL < 9.4
  QMAKE_CXXFLAGS+="-DHAVE_STRUCT_TIMESPEC"
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

