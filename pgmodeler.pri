################################################################################
# pgModeler
# General stuff
################################################################################

QT += core widgets printsupport network
CONFIG += ordered qt stl rtti exceptions warn_on c++11
TEMPLATE = subdirs

# Setting up the flag passed to compiler to build the demo version
defined(DEMO_VERSION, var): QMAKE_CXXFLAGS+="-DDEMO_VERSION"

unix {
  CONFIG += x11

  # By default stuff gets installed into /usr/local
  !defined(PREFIX, var): PREFIX = $$PWD/build

  #BINDIR = $$PREFIX/bin
  #PRIVATEBINDIR = $$PREFIX/lib/pgmodeler/bin
  #PRIVATELIBDIR = $$PREFIX/lib/pgmodeler
  #PLUGINSDIR = $$PREFIX/lib/pgmodeler/plugins
  #SHAREDIR = $$PREFIX/share/pgmodeler
  #CONFDIR = $$SHAREDIR/conf
  #DOCDIR = $$SHAREDIR/doc
  #LANGDIR = $$SHAREDIR/lang
  #SAMPLESDIR = $$SHAREDIR/samples
  #SCHEMASDIR = $$SHAREDIR/schemas

  BINDIR = $$PREFIX
  PRIVATEBINDIR = $$PREFIX
  PRIVATELIBDIR = $$PREFIX/lib
  PLUGINSDIR = $$PREFIX/lib/plugins
  SHAREDIR = $$PREFIX/share
  CONFDIR = $$SHAREDIR/conf
  DOCDIR = $$SHAREDIR/doc
  LANGDIR = $$SHAREDIR/lang
  SAMPLESDIR = $$SHAREDIR/samples
  SCHEMASDIR = $$SHAREDIR/schemas

  # Let the apps find the private libraries.
  QMAKE_RPATHDIR += $$PRIVATELIBDIR

  macx {
    CONFIG -= app_bundle
  }
}

windows {
  CONFIG += windows

  # On Windows we will install in deploy
  !defined(PREFIX, var): PREFIX = deploy

  BINDIR = $$PREFIX
  PRIVATEBINDIR = $$PREFIX
  PRIVATELIBDIR = $$PREFIX
  PLUGINSDIR = $$PREFIX
  SHAREDIR = $$PREFIX
  CONFDIR = conf
  DOCDIR = doc
  LANGDIR = lang
  SAMPLESDIR = samples
  SCHEMASDIR = schemas
}

# The paths are in globalattributes.h, which is a namespace, so we need to
# define this globally.
DEFINES += PLUGINSDIR=\\\"$${PLUGINSDIR}\\\" \
           PRIVATEBINDIR=\\\"$${PRIVATEBINDIR}\\\" \
           CONFDIR=\\\"$${CONFDIR}\\\" \
           DOCDIR=\\\"$${DOCDIR}\\\" \
           LANGDIR=\\\"$${LANGDIR}\\\" \
           SAMPLESDIR=\\\"$${SAMPLESDIR}\\\" \
           SCHEMASDIR=\\\"$${SCHEMASDIR}\\\"

################################################################################
# Dependencies
#
#
# PGSQL_LIB -> Full path to libpq.(so | dll | dylib)
# PGSQL_INC -> Root path where PgSQL includes can be found
#
# XML_LIB   -> Full path to libxml2.(so | dll | dylib)
# XML_INC   -> Root path where XML2 includes can be found
################################################################################

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
  XML_INC = C:/Qt/Qt5.3.2/5.3/mingw482_32/include
  XML_LIB = C:/Qt/Qt5.3.2/5.3/mingw482_32/bin/libxml2-2.dll
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

