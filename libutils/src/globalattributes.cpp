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

const QString GlobalAttributes::PgModelerVersion = QString("0.9.3-alpha")
/* Appending the snapshot build number to the version number
 * when the external variable SNAPSHOT_BUILD is defined */
#if defined(SNAPSHOT_BUILD)
		+ QString("_snapshot") + BUILDNUM
#endif
;
/****/

const QString GlobalAttributes::PgModelerAppName=QString("pgmodeler");
const QString GlobalAttributes::PgModelerURI=QString("pgmodeler.io");
const QString GlobalAttributes::PgModelerReverseURI=QString("io.pgmodeler");
const QString GlobalAttributes::PgModelerBuildNumber=QString(BUILDNUM);
const QString GlobalAttributes::PgModelerSite=QString("https://pgmodeler.io");
const QString GlobalAttributes::PgModelerSupport=QString("https://pgmodeler.io/support/docs");
const QString GlobalAttributes::PgModelerSourceURL=QString("https://github.com/pgmodeler/pgmodeler/releases");
const QString GlobalAttributes::PgModelerDownloadURL=QString("%1/download").arg(PgModelerSite);
const QString GlobalAttributes::PgModelerDonateURL=QString("%1/#donationForm").arg(PgModelerSite);
const QString GlobalAttributes::PgModelerUpdateCheckURL=QString("%1/checkupdate?version=").arg(PgModelerSite);
const QString GlobalAttributes::BugReportEmail=QString("bug@pgmodeler.io");
const QString GlobalAttributes::BugReportFile=QString("pgmodeler%1.bug");
const QString GlobalAttributes::StacktraceFile=QString(".stacktrace");

const QString GlobalAttributes::DirSeparator=QString("/");
const QString GlobalAttributes::DefaultConfsDir=QString("defaults");
const QString GlobalAttributes::ConfsBackupsDir=QString("backups");
const QString GlobalAttributes::SchemasDir=QString("schemas");
const QString GlobalAttributes::SQLSchemaDir=QString("sql");
const QString GlobalAttributes::XMLSchemaDir=QString("xml");
const QString GlobalAttributes::DataDictSchemaDir=QString("datadict");
const QString GlobalAttributes::AlterSchemaDir=QString("alter");
const QString GlobalAttributes::SchemaExt=QString(".sch");
const QString GlobalAttributes::ObjectDTDDir=QString("dtd");
const QString GlobalAttributes::ObjectDTDExt=QString(".dtd");
const QString GlobalAttributes::RootDTD=QString("dbmodel");
const QString GlobalAttributes::MetadataDTD=QString("metadata");
const QString GlobalAttributes::ConfigurationExt=QString(".conf");
const QString GlobalAttributes::HighlightFileSuffix=QString("-highlight");

const QString GlobalAttributes::CodeHighlightConf=QString("source-code-highlight");
const QString GlobalAttributes::ObjectsStyleConf=QString("objects-style");
const QString GlobalAttributes::GeneralConf=QString("pgmodeler");
const QString GlobalAttributes::ConnectionsConf=QString("connections");
const QString GlobalAttributes::RelationshipsConf=QString("relationships");
const QString GlobalAttributes::SnippetsConf=QString("snippets");
const QString GlobalAttributes::SQLHistoryConf=QString("sql-history");
const QString GlobalAttributes::DiffPresetsConf=QString("diff-presets");

const QString GlobalAttributes::SQLHighlightConf=QString("sql-highlight");
const QString GlobalAttributes::XMLHighlightConf=QString("xml-highlight");
const QString GlobalAttributes::PatternHighlightConf=QString("pattern-highlight");

const QString GlobalAttributes::ExampleModel=QString("example.dbm");
const QString GlobalAttributes::UiStyleConf=QString("ui-style");

const QString GlobalAttributes::DefaultQtStyle=QString("Fusion");
const QString GlobalAttributes::UiStyleOption=QString("-style");

QString GlobalAttributes::SchemasRootDir=GlobalAttributes::getPathFromEnv(QString("PGMODELER_SCHEMAS_DIR"), QString(SCHEMASDIR), QString("./schemas"));
QString GlobalAttributes::LanguagesDir=GlobalAttributes::getPathFromEnv(QString("PGMODELER_LANG_DIR"), QString(LANGDIR), QString("./lang"));
QString GlobalAttributes::SamplesDir=getPathFromEnv(QString("PGMODELER_SAMPLES_DIR"), QString(SAMPLESDIR), QString("./samples"));
QString GlobalAttributes::TmplConfigurationDir=getPathFromEnv(QString("PGMODELER_TMPL_CONF_DIR"), QString(CONFDIR), QString("./conf"));
QString GlobalAttributes::PluginsDir=getenv("PGMODELER_PLUGINS_DIR") ? QString(getenv("PGMODELER_PLUGINS_DIR")).replace('\\','/') : QString(PLUGINSDIR);

#if defined(Q_OS_MAC)
	QString GlobalAttributes::ConfigurationsDir=getPathFromEnv(QString("PGMODELER_CONF_DIR"), QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QString("/%1").arg(PgModelerReverseURI));
	QString GlobalAttributes::TemporaryDir=getPathFromEnv(QString("PGMODELER_TMP_DIR"), QStandardPaths::writableLocation(QStandardPaths::ConfigLocation)  + QString("/%1/tmp").arg(PgModelerReverseURI));
#elif defined(Q_OS_LINUX)
	QString GlobalAttributes::ConfigurationsDir=getPathFromEnv(QString("PGMODELER_CONF_DIR"), QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QString("/%1").arg(PgModelerAppName));
	QString GlobalAttributes::TemporaryDir=getPathFromEnv(QString("PGMODELER_TMP_DIR"), QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QString("/%1/tmp").arg(PgModelerAppName));
#else
	QString GlobalAttributes::ConfigurationsDir=getPathFromEnv(QString("PGMODELER_CONF_DIR"), QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QString("/%1").arg(PgModelerAppName));
	TemporaryDir=getPathFromEnv(QString("PGMODELER_TMP_DIR"), QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QString("/%1/tmp").arg(PgModelerAppName));
#endif

	QString GlobalAttributes::SQLHighlightConfPath=ConfigurationsDir + DirSeparator + SQLHighlightConf + ConfigurationExt;
	QString GlobalAttributes::XMLHighlightConfPath=ConfigurationsDir + DirSeparator + XMLHighlightConf + ConfigurationExt;

#if defined(Q_OS_UNIX)
	#if defined(Q_OS_MAC)
		//For MacOSX the crash handler path is fixed (inside bundle)
		QString GlobalAttributes::PgModelerCHandlerPath=getPathFromEnv(QString("PGMODELER_CHANDLER_PATH"), QString(BINDIR) + QString("/pgmodeler-ch"), QString("./pgmodeler-ch"));
	#else
		QString GlobalAttributes::PgModelerCHandlerPath=getPathFromEnv(QString("PGMODELER_CHANDLER_PATH"), QString(PRIVATEBINDIR) + QString("/pgmodeler-ch"), QString("./pgmodeler-ch"));
	#endif

	QString GlobalAttributes::PgModelerCLIPath=getPathFromEnv(QString("PGMODELER_CLI_PATH"), QString(BINDIR) + QString("/pgmodeler-cli"), QString("./pgmodeler-cli"));
	QString GlobalAttributes::PgModelerAppPath=getPathFromEnv(QString("PGMODELER_APP_PATH"), QString(BINDIR) + QString("/pgmodeler"), QString("./pgmodeler"));

#else
	QString GlobalAttributes::PgModelerCHandlerPath=getPathFromEnv(QString("PGMODELER_CHANDLER_PATH"), QString(PRIVATEBINDIR) + QString("\\pgmodeler-ch.exe"), QString(".\\pgmodeler-ch.exe"));
	QString GlobalAttributes::PgModelerCLIPath=getPathFromEnv(QString("PGMODELER_CLI_PATH"), QString(PRIVATEBINDIR) + QString("\\pgmodeler-cli.exe"), QString(".\\pgmodeler-cli.exe"));
	QString GlobalAttributes::PgModelerAppPath=getPathFromEnv(QString("PGMODELER_APP_PATH"), QString(BINDIR) + QString("\\pgmodeler.exe"), QString(".\\pgmodeler.exe"));
#endif

QString GlobalAttributes::getPathFromEnv(const QString &varname, const QString &default_val, const QString &fallback_val)
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

QString GlobalAttributes::getSchemasRootDir(void)
{
	return(SchemasRootDir);
}

QString GlobalAttributes::getLanguagesDir(void)
{
	return(LanguagesDir);
}

QString GlobalAttributes::getPluginsDir(void)
{
	return(PluginsDir);
}

QString GlobalAttributes::getTemporaryDir(void)
{
	return(TemporaryDir);
}

QString GlobalAttributes::getSamplesDir(void)
{
	return(SamplesDir);
}

QString GlobalAttributes::getTmplConfigurationDir(void)
{
	return(TmplConfigurationDir);
}

QString GlobalAttributes::getConfigurationsDir(void)
{
	return(ConfigurationsDir);
}

QString GlobalAttributes::getSQLHighlightConfPath(void)
{
	return(SQLHighlightConfPath);
}

QString GlobalAttributes::getXMLHighlightConfPath(void)
{
	return(XMLHighlightConfPath);
}

QString GlobalAttributes::getPgModelerCHandlerPath(void)
{
	return(PgModelerCHandlerPath);
}

QString GlobalAttributes::getPgModelerCLIPath(void)
{
	return(PgModelerCLIPath);
}

QString GlobalAttributes::getPgModelerAppPath(void)
{
	return(PgModelerAppPath);
}
