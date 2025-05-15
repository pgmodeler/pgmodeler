set(TARGET connector)

set(CMAKE_AUTOUIC OFF)
set(CMAKE_AUTOMOC ON)

add_library(${TARGET} SHARED
    src/catalog.cpp src/catalog.h
    src/connection.cpp src/connection.h
    src/connectorglobal.h
    src/resultset.cpp src/resultset.h)

target_include_directories(${TARGET} PUBLIC
    ${LIBCORE_INC}
    ${LIBPARSERS_INC}
    ${LIBUTILS_INC}
    ${PGSQL_INC})

target_link_libraries(${TARGET} PUBLIC
    ${PGSQL_LIBS}
    core
    parsers
    utils)

pgm_install_library(${TARGET})
