/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2020 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

const QString GlobalAttributes::PgModelerVersion=QString("0.9.3-alpha1")
/* Appending the snapshot build number to the version number
 * when the external variable SNAPSHOT_BUILD is defined */
#if defined(SNAPSHOT_BUILD)
		+ QString("_snapshot") + BUILDNUM
#endif
;
/****/

const QString GlobalAttributes::PgModelerAppName("pgmodeler");
const QString GlobalAttributes::PgModelerURI("pgmodeler.io");
const QString GlobalAttributes::PgModelerReverseURI("io.pgmodeler");
const QString GlobalAttributes::PgModelerBuildNumber(BUILDNUM);
const QString GlobalAttributes::PgModelerSite("https://pgmodeler.io");
const QString GlobalAttributes::PgModelerSupport("https://pgmodeler.io/support/docs");
const QString GlobalAttributes::PgModelerSourceURL("https://github.com/pgmodeler/pgmodeler/releases");
const QString GlobalAttributes::PgModelerDownloadURL=QString("%1/download").arg(PgModelerSite);
const QString GlobalAttributes::PgModelerDonateURL=QString("%1/#donationForm").arg(PgModelerSite);
const QString GlobalAttributes::PgModelerUpdateCheckURL=QString("%1/checkupdate?version=").arg(PgModelerSite);
const QString GlobalAttributes::BugReportEmail("bug@pgmodeler.io");
const QString GlobalAttributes::BugReportFile("pgmodeler%1.bug");
const QString GlobalAttributes::StacktraceFile(".stacktrace");

const QString GlobalAttributes::DirSeparator("/");
const QString GlobalAttributes::DefaultConfsDir("defaults");
const QString GlobalAttributes::ConfsBackupsDir("backups");
const QString GlobalAttributes::SchemasDir("schemas");
const QString GlobalAttributes::SQLSchemaDir("sql");
const QString GlobalAttributes::XMLSchemaDir("xml");
const QString GlobalAttributes::CatalogSchemasDir("catalog");
const QString GlobalAttributes::DataDictSchemaDir("datadict");
const QString GlobalAttributes::AlterSchemaDir("alter");
const QString GlobalAttributes::SchemaExt(".sch");
const QString GlobalAttributes::ObjectDTDDir("dtd");
const QString GlobalAttributes::ObjectDTDExt(".dtd");
const QString GlobalAttributes::RootDTD("dbmodel");
const QString GlobalAttributes::MetadataDTD("metadata");
const QString GlobalAttributes::ConfigurationExt(".conf");
const QString GlobalAttributes::HighlightFileSuffix("-highlight");

const QString GlobalAttributes::CodeHighlightConf("source-code-highlight");
const QString GlobalAttributes::ObjectsStyleConf("objects-style");
const QString GlobalAttributes::GeneralConf("pgmodeler");
const QString GlobalAttributes::ConnectionsConf("connections");
const QString GlobalAttributes::RelationshipsConf("relationships");
const QString GlobalAttributes::SnippetsConf("snippets");
const QString GlobalAttributes::SQLHistoryConf("sql-history");
const QString GlobalAttributes::DiffPresetsConf("diff-presets");

const QString GlobalAttributes::SQLHighlightConf("sql-highlight");
const QString GlobalAttributes::XMLHighlightConf("xml-highlight");
const QString GlobalAttributes::PatternHighlightConf("pattern-highlight");

const QString GlobalAttributes::ExampleModel("example.dbm");
const QString GlobalAttributes::UiStyleConf("ui-style");

const QString GlobalAttributes::DefaultQtStyle("Fusion");
const QString GlobalAttributes::UiStyleOption("-style");

QString GlobalAttributes::SchemasRootDir;
QString GlobalAttributes::LanguagesDir;
QString GlobalAttributes::SamplesDir;
QString GlobalAttributes::TmplConfigurationDir;
QString GlobalAttributes::PluginsDir;
QString GlobalAttributes::ConfigurationsDir;
QString GlobalAttributes::TemporaryDir;
QString GlobalAttributes::SQLHighlightConfPath;
QString GlobalAttributes::XMLHighlightConfPath;
QString GlobalAttributes::PgModelerCLIPath;
QString GlobalAttributes::PgModelerAppPath;
QString GlobalAttributes::PgModelerCHandlerPath;

QString GlobalAttributes::getPathFromEnv(const QString &varname, const QString &default_val, const QString &fallback_val)
{
	QFileInfo fi;
	QStringList paths={ QDir::toNativeSeparators(getenv(varname.toStdString().c_str())),
											QDir::toNativeSeparators(default_val) };

	for(int i=0; i < 2; i++)
	{
		fi.setFile(paths[i]);
		if(fi.exists() || (i==1 && fallback_val.isEmpty()))
			return paths[i].replace('\\','/');
	}

	fi.setFile(fallback_val);
	return fi.absoluteFilePath();
}

QString GlobalAttributes::getSchemasRootDir()
{
	return SchemasRootDir;
}

QString GlobalAttributes::getSchemaFilePath(const QString &subfolder, const QString &file)
{
	return SchemasRootDir + DirSeparator +
				 (subfolder.isEmpty() ? "" : subfolder + DirSeparator) +
				 file + SchemaExt;
}

QString GlobalAttributes::getLanguagesDir()
{
	return LanguagesDir;
}

QString GlobalAttributes::getPluginsDir()
{
	return PluginsDir;
}

QString GlobalAttributes::getTemporaryDir()
{
	return TemporaryDir;
}

QString GlobalAttributes::getTemporaryFilePath(const QString &file)
{
	return TemporaryDir + DirSeparator + file;
}

QString GlobalAttributes::getSamplesDir()
{
	return SamplesDir;
}

QString GlobalAttributes::getTmplConfigurationDir()
{
	return TmplConfigurationDir;
}

QString GlobalAttributes::getTmplConfigurationFilePath(const QString &subfolder, const QString &file)
{
	return TmplConfigurationDir + DirSeparator +
				 (subfolder.isEmpty() ? "" : subfolder + DirSeparator) + file;
}

QString GlobalAttributes::getConfigurationsDir()
{
	return ConfigurationsDir;
}

QString GlobalAttributes::getConfigurationFilePath(const QString &file)
{
	return ConfigurationsDir + DirSeparator + file + ConfigurationExt;
}

QString GlobalAttributes::getSQLHighlightConfPath()
{
	return SQLHighlightConfPath;
}

QString GlobalAttributes::getXMLHighlightConfPath()
{
	return XMLHighlightConfPath;
}

QString GlobalAttributes::getPgModelerCHandlerPath()
{
	return PgModelerCHandlerPath;
}

QString GlobalAttributes::getPgModelerCLIPath()
{
	return PgModelerCLIPath;
}

QString GlobalAttributes::getPgModelerAppPath()
{
	return PgModelerAppPath;
}

void GlobalAttributes::setSearchPath(const QString &search_path)
{
	SchemasRootDir=GlobalAttributes::getPathFromEnv("PGMODELER_SCHEMAS_DIR", SCHEMASDIR, QString("%1/schemas").arg(search_path));
	LanguagesDir=GlobalAttributes::getPathFromEnv("PGMODELER_LANG_DIR", LANGDIR, QString("%1/lang").arg(search_path));
	SamplesDir=getPathFromEnv("PGMODELER_SAMPLES_DIR", SAMPLESDIR, QString("%1/samples").arg(search_path));
	TmplConfigurationDir=getPathFromEnv("PGMODELER_TMPL_CONF_DIR", CONFDIR, QString("%1/conf").arg(search_path));
	PluginsDir=getenv("PGMODELER_PLUGINS_DIR") ? QString(getenv("PGMODELER_PLUGINS_DIR")).replace('\\','/') : PLUGINSDIR;

	#if defined(Q_OS_MAC)
		ConfigurationsDir=getPathFromEnv("PGMODELER_CONF_DIR", QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QString("/%1").arg(PgModelerReverseURI));
		TemporaryDir=getPathFromEnv("PGMODELER_TMP_DIR", QStandardPaths::writableLocation(QStandardPaths::ConfigLocation)  + QString("/%1/tmp").arg(PgModelerReverseURI));
	#elif defined(Q_OS_LINUX)
		ConfigurationsDir=getPathFromEnv("PGMODELER_CONF_DIR", QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QString("/%1").arg(PgModelerAppName));
		TemporaryDir=getPathFromEnv("PGMODELER_TMP_DIR", QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QString("/%1/tmp").arg(PgModelerAppName));
	#else
		ConfigurationsDir=getPathFromEnv("PGMODELER_CONF_DIR", QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QString("/%1").arg(PgModelerAppName));
		TemporaryDir=getPathFromEnv("PGMODELER_TMP_DIR", QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QString("/%1/tmp").arg(PgModelerAppName));
	#endif

	SQLHighlightConfPath=ConfigurationsDir + DirSeparator + SQLHighlightConf + ConfigurationExt;
	XMLHighlightConfPath=ConfigurationsDir + DirSeparator + XMLHighlightConf + ConfigurationExt;

	#if defined(Q_OS_UNIX)
		#if defined(Q_OS_MAC)
			//For MacOSX the crash handler path is fixed (inside bundle)
			PgModelerCHandlerPath=getPathFromEnv("PGMODELER_CHANDLER_PATH", QString("%1/pgmodeler-ch").arg(BINDIR), QString("%1/pgmodeler-ch").arg(search_path));
		#else
			PgModelerCHandlerPath=getPathFromEnv("PGMODELER_CHANDLER_PATH", QString("%1/pgmodeler-ch").arg(PRIVATEBINDIR), QString("%1/pgmodeler-ch").arg(search_path));
		#endif

		PgModelerCLIPath=getPathFromEnv("PGMODELER_CLI_PATH", QString("%1/pgmodeler-cli").arg(BINDIR), QString("%1/pgmodeler-cli").arg(search_path));
		PgModelerAppPath=getPathFromEnv("PGMODELER_APP_PATH", QString("%1/pgmodeler").arg(BINDIR), QString("%1/pgmodeler").arg(search_path));

	#else
		PgModelerCHandlerPath=getPathFromEnv("PGMODELER_CHANDLER_PATH", QString("%1\\pgmodeler-ch.exe").arg(PRIVATEBINDIR), QString("%1\\pgmodeler-ch.exe").arg(search_path));
		PgModelerCLIPath=getPathFromEnv("PGMODELER_CLI_PATH", QString("%1\\pgmodeler-cli.exe").arg(PRIVATEBINDIR), QString("%1\\pgmodeler-cli.exe").arg(search_path));
		PgModelerAppPath=getPathFromEnv("PGMODELER_APP_PATH", QString("%1\\pgmodeler.exe").arg(BINDIR), QString("%1\\pgmodeler.exe").arg(search_path));
	#endif
}
