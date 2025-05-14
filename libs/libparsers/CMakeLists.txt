set(TARGET parsers)

set(CMAKE_AUTOUIC OFF)
set(CMAKE_AUTOMOC ON)

add_library(${TARGET} SHARED
    src/attributes.h
    src/csvdocument.cpp src/csvdocument.h
    src/csvparser.cpp src/csvparser.h
    src/parsersglobal.h
    src/schemaparser.cpp src/schemaparser.h
    src/xmlparser.cpp src/xmlparser.h)

target_include_directories(${TARGET} PUBLIC
    ${LIBUTILS_INC}
    ${XML2_INC})

target_link_libraries(${TARGET} PUBLIC
    ${XML2_LIBS}
    utils)

install(TARGETS ${TARGET}
    LIBRARY DESTINATION ${PGM_PRIVATELIBDIR}
    FRAMEWORK DESTINATION ${PGM_PRIVATELIBDIR}
    RUNTIME DESTINATION ${PGM_PRIVATELIBDIR})
