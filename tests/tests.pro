# tests.pro (reviewed version)
#
# Refactored by: Lisandro Damián Nicanor Pérez Meyer <perezmeyer@gmail.com>
# Refactored code: https://github.com/perezmeyer/pgmodeler/tree/shared_libs
# Reviewed by: Raphal Araújo e Silva <raphael@pgmodeler.com.br>
#
# NOTE: Reviewed code is not a direct merge from refactored version but based upon the
# refactored code, containing almost all changes done by the refactoring author.
TEMPLATE = subdirs
SUBDIRS = src/main \
src/baseobjecttest \
src/roletest \
src/syntaxhighlightertest \
src/databasemodeltest \
src/schemaparsertest \
src/linenumberstest \
src/partrelationshiptest \
src/foreigndatawrappertest \
src/servertest \
src/usermappingtest \
src/datadicttest \
src/fileselectortest


