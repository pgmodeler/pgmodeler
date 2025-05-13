add_library(utils SHARED
    src/application.cpp src/application.h
    src/attribsmap.h
    src/customuistyle.cpp src/customuistyle.h
    src/doublenan.h
    src/enumtype.h
    src/exception.cpp src/exception.h
    src/globalattributes.cpp src/globalattributes.h
    src/pgmodelerplugin.cpp src/pgmodelerplugin.h
    src/pgsqlversions.cpp src/pgsqlversions.h
    src/utilsglobal.h
    src/utilsns.cpp src/utilsns.h)

target_include_directories(utils PUBLIC ../../plugins/src)

install(TARGETS utils
    LIBRARY DESTINATION ${PGM_PRIVATELIBDIR}
    FRAMEWORK DESTINATION ${PGM_PRIVATELIBDIR}
    RUNTIME DESTINATION ${PGM_PRIVATELIBDIR})
