add_library(connector SHARED
    src/catalog.cpp src/catalog.h
    src/connection.cpp src/connection.h
    src/connectorglobal.h
    src/resultset.cpp src/resultset.h)

target_include_directories(connector PUBLIC
    ${LIBCORE_INC}
    ${LIBPARSERS_INC}
    ${LIBUTILS_INC}
    ${PGSQL_INC}
    ../../plugins/src)

target_link_libraries(connector PUBLIC
    ${PGSQL_LIBS}
    core
    parsers
    utils)

install(TARGETS connector
    LIBRARY DESTINATION ${PGM_PRIVATELIBDIR}
    FRAMEWORK DESTINATION ${PGM_PRIVATELIBDIR}
    RUNTIME DESTINATION ${PGM_PRIVATELIBDIR})
