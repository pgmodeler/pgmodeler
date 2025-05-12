cmake_minimum_required(VERSION 3.16)
project(tests VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core)

qt_standard_project_setup()


add_subdirectory(src/main)
add_subdirectory(src/schemafilessyntaxtest)
add_subdirectory(src/baseobjecttest)
add_subdirectory(src/roletest)
add_subdirectory(src/syntaxhighlightertest)
add_subdirectory(src/databasemodeltest)
add_subdirectory(src/schemaparsertest)
add_subdirectory(src/linenumberstest)
add_subdirectory(src/partrelationshiptest)
add_subdirectory(src/foreigndatawrappertest)
add_subdirectory(src/servertest)
add_subdirectory(src/usermappingtest)
add_subdirectory(src/datadicttest)
add_subdirectory(src/fileselectortest)
add_subdirectory(src/transformtest)
add_subdirectory(src/xmlparsertest)
add_subdirectory(src/proceduretest)
add_subdirectory(src/basefunctiontest)
add_subdirectory(src/csvparsertest)
