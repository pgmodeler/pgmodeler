# Default configuration for package pgModeler.
# The default prefix is /usr/local
if(NOT DEFINED PGM_PREFIX)
    set(PGM_PREFIX ${CMAKE_INSTALL_PREFIX})
endif()

if(NOT DEFINED PGM_BINDIR)
    set(PGM_BINDIR ${PGM_PREFIX}/bin)
endif()

if(NOT DEFINED PGM_PRIVATEBINDIR)
    set(PGM_PRIVATEBINDIR ${PGM_PREFIX}/bin)
endif()

if(NOT DEFINED PGM_PRIVATELIBDIR)
    set(PGM_PRIVATELIBDIR ${PGM_PREFIX}/lib/pgmodeler)
endif()

if(NOT DEFINED PGM_PLUGINSDIR)
    set(PGM_PLUGINSDIR ${PGM_PREFIX}/lib/pgmodeler/plugins)
endif()

if(NOT DEFINED PGM_SHAREDIR)
    set(PGM_SHAREDIR ${PGM_PREFIX}/share/pgmodeler)
endif()

if(NOT DEFINED PGM_CONFDIR)
    set(PGM_CONFDIR ${PGM_PREFIX}/conf)
endif()

if(NOT DEFINED PGM_DOCDIR)
    set(PGM_DOCDIR ${PGM_SHAREDIR})
endif()

if(NOT DEFINED PGM_LANGDIR)
    set(PGM_LANGDIR ${PGM_PREFIX}/lang)
endif()

if(NOT DEFINED PGM_SAMPLESDIR)
    set(PGM_SAMPLESDIR ${PGM_PREFIX}/samples)
endif()

if(NOT DEFINED PGM_SCHEMASDIR)
    set(PGM_SCHEMASDIR ${PGM_PREFIX}/schemas)
endif()
