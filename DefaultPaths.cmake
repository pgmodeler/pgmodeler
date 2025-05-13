# Defining default values of the path-related variables
if(LINUX)
  # Default configuration for package pgModeler.
  # The default prefix is /usr/local
  if(NOT DEFINED PREFIX)
    set(PREFIX ${CMAKE_INSTALL_PREFIX})
  endif()

  if(NOT DEFINED BINDIR)
    set(BINDIR ${PREFIX}/bin)
  endif()

  if(NOT DEFINED PRIVATEBINDIR)
    set(PRIVATEBINDIR ${PREFIX}/bin)
  endif()

  if(NOT DEFINED PRIVATELIBDIR)
    set(PRIVATELIBDIR ${PREFIX}/lib/pgmodeler)
  endif()

  if(NOT DEFINED PLUGINSDIR)
    set(PLUGINSDIR ${PREFIX}/lib/pgmodeler/plugins)
  endif()

  if(NOT DEFINED SHAREDIR)
    set(SHAREDIR ${PREFIX}/share/pgmodeler)
  endif()

  if(NOT DEFINED CONFDIR)
    set(CONFDIR ${PREFIX}/conf)
  endif()

  if(NOT DEFINED DOCDIR)
    set(DOCDIR ${SHAREDIR})
  endif()

  if(NOT DEFINED LANGDIR)
    set(LANGDIR ${PREFIX}/lang)
  endif()

  if(NOT DEFINED SAMPLESDIR)
    set(SAMPLESDIR ${PREFIX}/samples)
  endif()

  if(NOT DEFINED SCHEMASDIR)
    set(SCHEMASDIR ${PREFIX}/schemas)
  endif()
endif()

add_compile_definitions(
    BINDIR="${BINDIR}"
    CONFDIR="${CONFDIR}"
    DOCDIR="${DOCDIR}"
    LANGDIR="${LANGDIR}"
    PLUGINSDIR="${PLUGINSDIR}"
    PRIVATEBINDIR="${PRIVATEBINDIR}"
    SAMPLESDIR="${SAMPLESDIR}"
    SCHEMASDIR="${SCHEMASDIR}"
)

# Searching for PostgreSQL headers/libraries
# This command attempts to find the library, REQUIRED argument is optional
find_package(PostgreSQL REQUIRED)
set(PGSQL_INC ${PostgreSQL_INCLUDE_DIRS})
set(PGSQL_LIBS ${PostgreSQL_LIBRARIES})

# Searching for libxml2 headers/libraries
# This command attempts to find the library, REQUIRED argument is optional
find_package(LibXml2 REQUIRED)
set(XML2_INC ${LIBXML2_INCLUDE_DIR})
set(XML2_LIBS ${LIBXML2_LIBRARY})

# Store the absolute paths to library subprojects to be referenced in other .pro files
# *_ROOT -> the path to the root folder of the subproject
# *_INC -> the path to the source code folder (src), used by the flag -I (INCLUDEPATH on qmake) passed to the compiler
set(CWD ${CMAKE_CURRENT_SOURCE_DIR})

set(LIBCANVAS libcanvas)
set(LIBCANVAS_ROOT ${CWD}/libs/${LIBCANVAS})
set(LIBCANVAS_INC ${LIBCANVAS_ROOT}/src)

set(LIBCONNECTOR libconnector)
set(LIBCONNECTOR_ROOT ${CWD}/libs/${LIBCONNECTOR})
set(LIBCONNECTOR_INC ${LIBCONNECTOR_ROOT}/src)

set(LIBCORE libcore)
set(LIBCORE_ROOT ${CWD}/libs/${LIBCORE})
set(LIBCORE_INC ${LIBCORE_ROOT}/src)

set(LIBPARSERS libparsers)
set(LIBPARSERS_ROOT ${CWD}/libs/${LIBPARSERS})
set(LIBPARSERS_INC ${LIBPARSERS_ROOT}/src)

set(LIBGUI libgui)
set(LIBGUI_ROOT ${CWD}/libs/${LIBGUI})
set(LIBGUI_INC ${LIBGUI_ROOT}/src)

set(LIBUTILS libutils)
set(LIBUTILS_ROOT ${CWD}/libs/${LIBUTILS})
set(LIBUTILS_INC ${LIBUTILS_ROOT}/src)

set(LIBCLI libcli)
set(LIBCLI_ROOT ${CWD}/libs/${LIBCLI})
set(LIBCLI_INC ${LIBCLI_ROOT}/src)

# Configuring the relative paths so the binaries can find the libraries correclty
# First we determine the relative path between BINDIR e PRIVATELIBDIR
file(RELATIVE_PATH RELATIVE_PRIVATELIBDIR ${BINDIR} ${PRIVATELIBDIR})

# Setting the RPATH to including $ORIGIN and $ORIGIN/relative_path
set(CMAKE_INSTALL_RPATH "\$ORIGIN;\$ORIGIN/${RELATIVE_PRIVATELIBDIR}")

# Making sure that RPATH is used during install
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
