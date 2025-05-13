set(utils_src
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
    src/utilsns.cpp src/utilsns.h
)

qt_add_library(utils SHARED
    ${utils_src}
)

target_include_directories(utils PUBLIC
    ../../plugins/src
)

target_link_libraries(utils PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Network
    Qt::PrintSupport
    Qt::Svg
    Qt::Widgets
)

install(TARGETS utils
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
