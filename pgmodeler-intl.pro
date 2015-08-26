TEMPLATE = subdirs

SUBDIRS = libutils \
	  libparsers \
	  libdbconnect \
	  libpgmodeler \
	  libobjrenderer \
	  libpgmodeler_ui \
	  crashhandler \
	  main \
	  main-cli

TRANSLATIONS = lang/pt_BR.ts \
               lang/zh_CN.ts \
               lang/fr_FR.ts \
               lang/es_ES.ts \
               lang/nl_NL.ts

CODECFORTR = UTF8
