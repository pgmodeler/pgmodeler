TEMPLATE = subdirs

SUBDIRS = libs/libutils \
libs/libparsers \
libs/libconnector \
libs/libcore \
libs/libcanvas \
libs/libgui \
libs/libcli \
apps/pgmodeler-ch \
apps/pgmodeler \
apps/pgmodeler-cli \
apps/pgmodeler-se


TRANSLATIONS = assets/lang/pt_BR.ts \
               assets/lang/en_US.ts

CODECFORTR = UTF8
