/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2019 - Raphael Araújo e Silva <raphael@pgmodeler.io>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation version 3.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# The complete text of GPLv3 is at LICENSE file on source code root directory.
# Also, you can get the complete GNU General Public License at <http://www.gnu.org/licenses/>
*/

#include "globalattributes.h"
#include <QCoreApplication>
#include <QTextStream>

namespace GlobalAttributes {
	const QString
	PgModelerAppName=QString("pgmodeler"),
	PgModelerURI=QString("pgmodeler.io"),
	PGMODELER_REVERSE_URI=QString("io.pgmodeler"),
	PgModelerVersion=QString("0.9.2-beta"),
	PgModelerBuildNumber=QString(BUILDNUM),
	PgModelerSite=QString("https://pgmodeler.io"),
	PgModelerSupport=QString("https://pgmodeler.io/support/docs"),
	PgModelerSourceURL=QString("https://github.com/pgmodeler/pgmodeler/releases"),
	PgModelerDownloadURL=QString("%1/download").arg(PgModelerSite),
	PgModelerDonateURL=QString("%1/#donationForm").arg(PgModelerSite),
	PgModelerUpdateCheckURL=QString("%1/checkupdate?version=").arg(PgModelerSite),

	BugReportEmail=QString("bug@pgmodeler.io"),
	BugReportFile=QString("pgmodeler%1.bug"),
	StacktraceFile=QString(".stacktrace"),

	DirSeparator=QString("/"),
	DefaultConfsDir=QString("defaults"),
	ConfsBackupsDir=QString("backups"),
	SchemasDir=QString("schemas"),
	SQLSchemaDir=QString("sql"),
	XMLSchemaDir=QString("xml"),
	AlterSchemaDir=QString("alter"),
	SchemaExt=QString(".sch"),
	ObjectDTDDir=QString("dtd"),
	ObjectDTDExt=QString(".dtd"),
	RootDTD=QString("dbmodel"),
	MetadataDTD=QString("metadata"),
	ConfigurationExt=QString(".conf"),
	HighlightFileSuffix=QString("-highlight"),

	CodeHighlightConf=QString("source-code-highlight"),
	ObjectsStyleConf=QString("objects-style"),
	GeneralConf=QString("pgmodeler"),
	ConnectionsConf=QString("connections"),
	RelationshipsConf=QString("relationships"),
	SnippetsConf=QString("snippets"),
	SQLHistoryConf=QString("sql-history"),

	SQLHighlightConf=QString("sql-highlight"),
	XMLHighlightConf=QString("xml-highlight"),
	PatternHighlightConf=QString("pattern-highlight"),

	ExampleModel=QString("example.dbm"),
	UiStyleConf=QString("ui-style"),

	DefaultQtStyle=QString("Fusion"),
	UiStyleOption=QString("-style"),

	SchemasRootDir=getPathFromEnv(QString("PGMODELER_SCHEMAS_DIR"), QString(SCHEMASDIR), QString("./schemas")),
	LanguagesDir=getPathFromEnv(QString("PGMODELER_LANG_DIR"), QString(LANGDIR), QString("./lang")),
	SamplesDir=getPathFromEnv(QString("PGMODELER_SAMPLES_DIR"), QString(SAMPLESDIR), QString("./samples")),
	TmplConfigurationDir=getPathFromEnv(QString("PGMODELER_TMPL_CONF_DIR"), QString(CONFDIR), QString("./conf")),

	//Currently, plugins folder is auto-created when missing so it can't be resolved by getPathFromEnv()
	PluginsDir=getenv("PGMODELER_PLUGINS_DIR") ? QString(getenv("PGMODELER_PLUGINS_DIR")).replace('\\','/') : QString(PLUGINSDIR),

#if defined(Q_OS_MAC)
	ConfigurationsDir=getPathFromEnv(QString("PGMODELER_CONF_DIR"),
									  QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QString("/%1").arg(PGMODELER_REVERSE_URI)),

	TemporaryDir=getPathFromEnv(QString("PGMODELER_TMP_DIR"),
								 QStandardPaths::writableLocation(QStandardPaths::ConfigLocation)  + QString("/%1/tmp").arg(PGMODELER_REVERSE_URI)),

#elif defined(Q_OS_LINUX)
	ConfigurationsDir=getPathFromEnv(QString("PGMODELER_CONF_DIR"),
									  QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QString("/%1").arg(PgModelerAppName)),

	TemporaryDir=getPathFromEnv(QString("PGMODELER_TMP_DIR"),
								 QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QString("/%1/tmp").arg(PgModelerAppName)),
#else
	ConfigurationsDir=getPathFromEnv(QString("PGMODELER_CONF_DIR"),
                                      QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) +  QString("/%1").arg(PgModelerAppName)),

	TemporaryDir=getPathFromEnv(QString("PGMODELER_TMP_DIR"),
                                 QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QString("/%1/tmp").arg(PgModelerAppName)),
#endif

	SQLHighlightConfPath=ConfigurationsDir + DirSeparator + SQLHighlightConf + ConfigurationExt,
	XMLHighlightConfPath=ConfigurationsDir + DirSeparator + XMLHighlightConf + ConfigurationExt,

#if defined(Q_OS_UNIX)
#if defined(Q_OS_MAC)
	//For MacOSX the crash handler path is fixed (inside bundle)
	PgModelerCHandlerPath=getPathFromEnv(QString("PGMODELER_CHANDLER_PATH"),
										   QString(BINDIR) + QString("/pgmodeler-ch"),
										   QString("./pgmodeler-ch")),
#else
	PgModelerCHandlerPath=getPathFromEnv(QString("PGMODELER_CHANDLER_PATH"),
										   QString(PRIVATEBINDIR) + QString("/pgmodeler-ch"),
										   QString("./pgmodeler-ch")),
#endif

	PgModelerCLIPath=getPathFromEnv(QString("PGMODELER_CLI_PATH"),
									  QString(BINDIR) + QString("/pgmodeler-cli"),
									  QString("./pgmodeler-cli")),

	PgModelerAppPath=getPathFromEnv(QString("PGMODELER_APP_PATH"),
									  QString(BINDIR) + QString("/pgmodeler"),
									  QString("./pgmodeler"));


#else
	PgModelerCHandlerPath=getPathFromEnv(QString("PGMODELER_CHANDLER_PATH"),
										   QString(PRIVATEBINDIR) + QString("\\pgmodeler-ch.exe"),
										   QString(".\\pgmodeler-ch.exe")),

	PgModelerCLIPath=getPathFromEnv(QString("PGMODELER_CLI_PATH"),
									  QString(PRIVATEBINDIR) + QString("\\pgmodeler-cli.exe"),
									  QString(".\\pgmodeler-cli.exe")),

	PgModelerAppPath=getPathFromEnv(QString("PGMODELER_APP_PATH"),
									  QString(BINDIR) + QString("\\pgmodeler.exe"),
									  QString(".\\pgmodeler.exe"));

#endif

#ifdef DEMO_VERSION
	//Maximum object creation counter for demo version
	const unsigned MaxObjectCount=15;
#endif


	QString getPathFromEnv(const QString &varname, const QString &default_val, const QString &fallback_val)
	{
		QFileInfo fi;
		QStringList paths={ QDir::toNativeSeparators(getenv(varname.toStdString().c_str())),
							QDir::toNativeSeparators(default_val) };

		for(int i=0; i < 2; i++)
		{
			fi.setFile(paths[i]);
			if(fi.exists() || (i==1 && fallback_val.isEmpty()))
				return(paths[i].replace('\\','/'));
		}

		fi.setFile(fallback_val);
		return(fi.absoluteFilePath());
	}
}
