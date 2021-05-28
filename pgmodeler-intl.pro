TEMPLATE = subdirs

SUBDIRS = pgmutils \
	  pgmparsers \
	  pgmconnector \
	  pgmcore \
	  pgmcanvas \
	  pgmui \
	  apps/crashhandler \
	  apps/gui \
	  app/cli

TRANSLATIONS = assets/lang/pt_BR.ts \
               assets/lang/zh_CN.ts \
               assets/lang/fr_FR.ts \
               assets/lang/es_ES.ts \
               assets/lang/nl_NL.ts

CODECFORTR = UTF8
