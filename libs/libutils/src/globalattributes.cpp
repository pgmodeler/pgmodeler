/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2022 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
#include <QDir>
#include <QRegularExpression>

/** Base version number **/
const QString GlobalAttributes::PgModelerVersion = QString("1.0.0-alpha1")

/* Appending the snapshot build number to the version number
 * when the external variable SNAPSHOT_BUILD is defined */
#if defined(SNAPSHOT_BUILD)
	+ QString("_snapshot%1").arg(BUILDDATE)
#endif
;
/****/

const QString GlobalAttributes::PgModelerBuildNumber(QString("%1.%2").arg(BUILDDATE).arg(BUILDNUM));
const QString GlobalAttributes::PgModelerAppName("pgmodeler-1.0");
const QString GlobalAttributes::PgModelerURI("pgmodeler.io");
const QString GlobalAttributes::PgModelerSite("https://pgmodeler.io");
const QString GlobalAttributes::PgModelerSupport("https://pgmodeler.io/support/docs");
const QString GlobalAttributes::PgModelerSourceURL("https://github.com/pgmodeler/pgmodeler/releases");
const QString GlobalAttributes::PgModelerDownloadURL=PgModelerSite + "/download";
const QString GlobalAttributes::PgModelerDonateURL=PgModelerSite + "/#donationForm";
const QString GlobalAttributes::PgModelerUpdateCheckURL=PgModelerSite + "/checkupdate?version=%1&mode=%2";
const QString GlobalAttributes::BugReportEmail("bug@pgmodeler.io");
const QString GlobalAttributes::BugReportFile("pgmodeler%1.bug");
const QString GlobalAttributes::StacktraceFile(".stacktrace");
const QString GlobalAttributes::LastModelFile("lastmodel");

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
const QString GlobalAttributes::ThemesDir("themes");

const QString GlobalAttributes::CodeHighlightConf("source-code-highlight");
const QString GlobalAttributes::AppearanceConf("appearance");
const QString GlobalAttributes::GeneralConf("general");
const QString GlobalAttributes::ConnectionsConf("connections");
const QString GlobalAttributes::RelationshipsConf("relationships");
const QString GlobalAttributes::SnippetsConf("snippets");
const QString GlobalAttributes::SQLHistoryConf("sql-history");
const QString GlobalAttributes::DiffPresetsConf("diff-presets");

const QString GlobalAttributes::SQLHighlightConf("sql-highlight");
const QString GlobalAttributes::XMLHighlightConf("xml-highlight");
const QString GlobalAttributes::SchHighlightConf("sch-highlight");
const QString GlobalAttributes::PatternHighlightConf("pattern-highlight");

const QString GlobalAttributes::ExampleModel("example.dbm");
const QString GlobalAttributes::UiStyleConf("ui-style");
const QString GlobalAttributes::IconsMediumConf("icons-medium");
const QString GlobalAttributes::IconsSmallConf("icons-small");
const QString GlobalAttributes::IconsBigConf("icons-big");

const QString GlobalAttributes::FileDialogConf("filedlg");

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
QString GlobalAttributes::SchHighlightConfPath;
QString GlobalAttributes::PatternHighlightConfPath;
QString GlobalAttributes::PgModelerCLIPath;
QString GlobalAttributes::PgModelerAppPath;
QString GlobalAttributes::PgModelerCHandlerPath;
QString GlobalAttributes::PgModelerSchemaEditorPath;

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

QString GlobalAttributes::getSchHighlightConfPath()
{
	return SchHighlightConfPath;
}

QString GlobalAttributes::getPatternHighlightConfPath()
{
	return PatternHighlightConfPath;
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

QString GlobalAttributes::getPgModelerSchemaEditorPath()
{
	return PgModelerSchemaEditorPath;
}

QString GlobalAttributes::getConfigParamFromFile(const QString &param_name, const QString &conf_file)
{
	setConfigFilesPaths();

	QString filename = getConfigurationFilePath(conf_file);
	QFile input;
	QString attr_val;

	input.setFileName(filename);

	if(input.open(QFile::ReadOnly) && !param_name.isEmpty())
	{
		QString buf = QString(input.readAll());
		QRegularExpression regexp = QRegularExpression(QString("(%1)(.*)(=)(\\\")(.)+(\\\")(\\\n)*").arg(param_name));
		QRegularExpressionMatch match;
		int idx =	-1;

		match =	regexp.match(buf);
		idx = match.capturedStart();

		//Extract the value of the attribute in the conf file
		attr_val = buf.mid(idx, match.capturedLength());
		attr_val.remove(param_name);
		attr_val.remove(QChar('"')).remove(QChar('=')).remove(QChar('\n'));
	}

	return attr_val;
}

void GlobalAttributes::setConfigFilesPaths()
{
	#if defined(Q_OS_WINDOWS)
		ConfigurationsDir=getPathFromEnv("PGMODELER_CONF_DIR", QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QString("/%1").arg(PgModelerAppName));
		TemporaryDir=getPathFromEnv("PGMODELER_TMP_DIR", QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QString("/%1/tmp").arg(PgModelerAppName));
	#else
		ConfigurationsDir=getPathFromEnv("PGMODELER_CONF_DIR", QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QString("/%1").arg(PgModelerAppName));
		TemporaryDir=getPathFromEnv("PGMODELER_TMP_DIR", QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QString("/%1/tmp").arg(PgModelerAppName));
	#endif

	SQLHighlightConfPath=ConfigurationsDir + DirSeparator + SQLHighlightConf + ConfigurationExt;
	XMLHighlightConfPath=ConfigurationsDir + DirSeparator + XMLHighlightConf + ConfigurationExt;
	SchHighlightConfPath=ConfigurationsDir + DirSeparator + SchHighlightConf + ConfigurationExt;
	PatternHighlightConfPath=ConfigurationsDir + DirSeparator + PatternHighlightConf + ConfigurationExt;
}

void GlobalAttributes::setSearchPath(const QString &search_path)
{
	setConfigFilesPaths();

	SchemasRootDir=GlobalAttributes::getPathFromEnv("PGMODELER_SCHEMAS_DIR", SCHEMASDIR, QString("%1/schemas").arg(search_path));
	LanguagesDir=GlobalAttributes::getPathFromEnv("PGMODELER_LANG_DIR", LANGDIR, QString("%1/lang").arg(search_path));
	SamplesDir=getPathFromEnv("PGMODELER_SAMPLES_DIR", SAMPLESDIR, QString("%1/samples").arg(search_path));
	TmplConfigurationDir=getPathFromEnv("PGMODELER_TMPL_CONF_DIR", CONFDIR, QString("%1/conf").arg(search_path));
	PluginsDir=getPathFromEnv("PGMODELER_PLUGINS_DIR", PLUGINSDIR, QString("%1/plugins").arg(search_path));

	#if defined(Q_OS_UNIX)
		#if defined(Q_OS_MAC)
			//For MacOSX the crash handler path is fixed (inside bundle)
			PgModelerCHandlerPath=getPathFromEnv("PGMODELER_CH_PATH", QString("%1/pgmodeler-ch").arg(BINDIR), QString("%1/pgmodeler-ch").arg(search_path));
		#else
			PgModelerCHandlerPath=getPathFromEnv("PGMODELER_CH_PATH", QString("%1/pgmodeler-ch").arg(PRIVATEBINDIR), QString("%1/pgmodeler-ch").arg(search_path));
		#endif

		PgModelerCLIPath=getPathFromEnv("PGMODELER_CLI_PATH", QString("%1/pgmodeler-cli").arg(BINDIR), QString("%1/pgmodeler-cli").arg(search_path));
		PgModelerAppPath=getPathFromEnv("PGMODELER_PATH", QString("%1/pgmodeler").arg(BINDIR), QString("%1/pgmodeler").arg(search_path));
		PgModelerSchemaEditorPath=getPathFromEnv("PGMODELER_SE_PATH", QString("%1/pgmodeler-se").arg(BINDIR), QString("%1/pgmodeler-sc").arg(search_path));

	#else
		PgModelerCHandlerPath=getPathFromEnv("PGMODELER_CH_PATH", QString("%1\\pgmodeler-ch.exe").arg(PRIVATEBINDIR), QString("%1\\pgmodeler-ch.exe").arg(search_path));
		PgModelerCLIPath=getPathFromEnv("PGMODELER_CLI_PATH", QString("%1\\pgmodeler-cli.exe").arg(PRIVATEBINDIR), QString("%1\\pgmodeler-cli.exe").arg(search_path));
		PgModelerAppPath=getPathFromEnv("PGMODELER_PATH", QString("%1\\pgmodeler.exe").arg(BINDIR), QString("%1\\pgmodeler.exe").arg(search_path));
		PgModelerSchemaEditorPath=getPathFromEnv("PGMODELER_SE_PATH", QString("%1/pgmodeler-se.exe").arg(BINDIR), QString("%1/pgmodeler-sc.exe").arg(search_path));
	#endif
}

void GlobalAttributes::setCustomUiScaleFactor()
{
	QString scale = GlobalAttributes::getConfigParamFromFile("custom-scale", GlobalAttributes::AppearanceConf);

	if(scale.toDouble() > 0)
		qputenv("QT_SCALE_FACTOR", scale.toUtf8());
}
