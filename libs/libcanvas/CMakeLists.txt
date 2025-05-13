qt_add_library(canvas SHARED
    src/attributestoggleritem.cpp src/attributestoggleritem.h
    src/baseobjectview.cpp src/baseobjectview.h
    src/basetableview.cpp src/basetableview.h
    src/beziercurveitem.cpp src/beziercurveitem.h
    src/canvasglobal.h
    src/graphicalview.cpp src/graphicalview.h
    src/layeritem.cpp src/layeritem.h
    src/objectsscene.cpp src/objectsscene.h
    src/relationshipview.cpp src/relationshipview.h
    src/roundedrectitem.cpp src/roundedrectitem.h
    src/schemaview.cpp src/schemaview.h
    src/styledtextboxview.cpp src/styledtextboxview.h
    src/tableobjectview.cpp src/tableobjectview.h
    src/tabletitleview.cpp src/tabletitleview.h
    src/tableview.cpp src/tableview.h
    src/textboxview.cpp src/textboxview.h
    src/textpolygonitem.cpp src/textpolygonitem.h
)

target_include_directories(canvas PUBLIC
    ${LIBCORE_INC}
    ${LIBPARSERS_INC}
    ${LIBUTILS_INC}
    ../../plugins/src
)

target_link_libraries(canvas PUBLIC
    core
    parsers
    utils
)

install(TARGETS canvas
    LIBRARY DESTINATION ${PRIVATELIBDIR}
    FRAMEWORK DESTINATION ${PRIVATELIBDIR}
    RUNTIME DESTINATION ${PRIVATELIBDIR}
)
