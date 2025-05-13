qt_add_library(parsers SHARED
    src/attributes.h
    src/csvdocument.cpp src/csvdocument.h
    src/csvparser.cpp src/csvparser.h
    src/parsersglobal.h
    src/schemaparser.cpp src/schemaparser.h
    src/xmlparser.cpp src/xmlparser.h
)
target_include_directories(parsers PUBLIC
    ${LIBUTILS_INC}
    ${XML2_INC}
    ../../plugins/src
)

target_link_libraries(parsers PUBLIC
    ${XML2_LIBS}
    utils
)

install(TARGETS parsers
    LIBRARY DESTINATION ${PRIVATELIBDIR}
    FRAMEWORK DESTINATION ${PRIVATELIBDIR}
    RUNTIME DESTINATION ${PRIVATELIBDIR}
)
