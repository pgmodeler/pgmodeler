/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2023 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
#include "utilsns.h"
#include "exception.h"
#include <QSettings>

/** Base version number **/
const QString GlobalAttributes::PgModelerVersion = QString("1.1.0")

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
const QString GlobalAttributes::PgmPathsConfFile("pgmpaths");

const QString GlobalAttributes::EnvSchemasPath("PGMODELER_SCHEMAS_PATH");
const QString GlobalAttributes::EnvConfPath("PGMODELER_CONF_PATH");
const QString GlobalAttributes::EnvTmplConfPath("PGMODELER_TMPL_CONF_PATH");
const QString GlobalAttributes::EnvLangPath("PGMODELER_LANG_PATH");
const QString GlobalAttributes::EnvPluginsPath("PGMODELER_PLUGINS_PATH");
const QString GlobalAttributes::EnvTmpPath("PGMODELER_TMP_PATH");
const QString GlobalAttributes::EnvSamplesPath("PGMODELER_SAMPLES_PATH");
const QString GlobalAttributes::EnvPgModelerChPath("PGMODELER_CH_PATH");
const QString GlobalAttributes::EnvPgModelerCliPath("PGMODELER_CLI_PATH");
const QString GlobalAttributes::EnvPgModelerSePath("PGMODELER_SE_PATH");
const QString GlobalAttributes::EnvPgModelerPath("PGMODELER_PATH");

const QString GlobalAttributes::DbModelExt(".dbm");
const QString GlobalAttributes::DbModelBkpExt(".dbk");
const QString GlobalAttributes::ObjMetadataExt(".omf");
const QString GlobalAttributes::DirSeparator("/");
const QString GlobalAttributes::ResourcesDir("res");
const QString GlobalAttributes::ConfigurationsDir("conf");
const QString GlobalAttributes::DefaultConfsDir("defaults");
const QString GlobalAttributes::ConfsBackupsDir("backups");
const QString GlobalAttributes::SchemasDir("schemas");
const QString GlobalAttributes::SQLSchemaDir("sql");
const QString GlobalAttributes::XMLSchemaDir("xml");
const QString GlobalAttributes::CatalogSchemasDir("catalog");
const QString GlobalAttributes::DataDictSchemaDir("datadict");
const QString GlobalAttributes::AlterSchemaDir("alter");
const QString GlobalAttributes::LanguagesDir("lang");
const QString GlobalAttributes::SamplesDir("samples");
const QString GlobalAttributes::PluginsDir("plugins");
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

QString GlobalAttributes::SchemasRootPath;
QString GlobalAttributes::LanguagesPath;
QString GlobalAttributes::SamplesPath;
QString GlobalAttributes::TmplConfigurationPath;
QString GlobalAttributes::PluginsPath;
QString GlobalAttributes::ConfigurationsPath;
QString GlobalAttributes::TemporaryPath;
QString GlobalAttributes::SQLHighlightConfPath;
QString GlobalAttributes::XMLHighlightConfPath;
QString GlobalAttributes::SchHighlightConfPath;
QString GlobalAttributes::PatternHighlightConfPath;
QString GlobalAttributes::PgModelerCLIPath;
QString GlobalAttributes::PgModelerAppPath;
QString GlobalAttributes::PgModelerCHandlerPath;
QString GlobalAttributes::PgModelerSchemaEditorPath;

attribs_map GlobalAttributes::CustomPaths ;

QString GlobalAttributes::getPathFromEnv(const QString &varname, const QString &default_val, const QString &fallback_val)
{
	QFileInfo fi;
	QStringList paths={ CustomPaths[varname],
											QDir::toNativeSeparators(default_val) };

	for(int i=0; i < 2; i++)
	{
		fi.setFile(paths[i]);

		if(fi.exists() || (i == 1 && fallback_val.isEmpty()))
			return fi.absoluteFilePath();
	}

	fi.setFile(fallback_val);
	return fi.absoluteFilePath();
}

QString GlobalAttributes::getSchemasRootPath()
{
	return SchemasRootPath;
}

QString GlobalAttributes::getSchemaFilePath(const QString &subfolder, const QString &file)
{
	return SchemasRootPath + DirSeparator +
				 (subfolder.isEmpty() ? "" : subfolder + DirSeparator) +
				 file + SchemaExt;
}

QString GlobalAttributes::getLanguagesPath()
{
	return LanguagesPath;
}

QString GlobalAttributes::getPluginsPath()
{
	return PluginsPath;
}

QString GlobalAttributes::getTemporaryPath()
{
	return TemporaryPath;
}

QString GlobalAttributes::getTemporaryFilePath(const QString &file)
{
	return TemporaryPath + DirSeparator + file;
}

QString GlobalAttributes::getSamplesPath()
{
	return SamplesPath;
}

QString GlobalAttributes::getTmplConfigurationPath()
{
	return TmplConfigurationPath;
}

QString GlobalAttributes::getTmplConfigurationFilePath(const QString &subfolder, const QString &file)
{
	return TmplConfigurationPath + DirSeparator +
				 (subfolder.isEmpty() ? "" : subfolder + DirSeparator) + file;
}

QString GlobalAttributes::getConfigurationsPath()
{
	return ConfigurationsPath;
}

QString GlobalAttributes::getConfigurationFilePath(const QString &file)
{
	return ConfigurationsPath + DirSeparator + file + ConfigurationExt;
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
		ConfigurationsPath=getPathFromEnv(EnvConfPath, QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QString("/%1").arg(PgModelerAppName));
		TemporaryPath=getPathFromEnv(EnvTmpPath, QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QString("/%1/tmp").arg(PgModelerAppName));
	#else
		ConfigurationsPath=getPathFromEnv(EnvConfPath, QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QString("/%1").arg(PgModelerAppName));
		TemporaryPath=getPathFromEnv(EnvTmpPath, QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QString("/%1/tmp").arg(PgModelerAppName));
	#endif

	SQLHighlightConfPath=ConfigurationsPath + DirSeparator + SQLHighlightConf + ConfigurationExt;
	XMLHighlightConfPath=ConfigurationsPath + DirSeparator + XMLHighlightConf + ConfigurationExt;
	SchHighlightConfPath=ConfigurationsPath + DirSeparator + SchHighlightConf + ConfigurationExt;
	PatternHighlightConfPath=ConfigurationsPath + DirSeparator + PatternHighlightConf + ConfigurationExt;
}

void GlobalAttributes::setCustomPaths(const QString &search_path)
{
	QStringList env_vars = {
		EnvSchemasPath,	EnvConfPath,	EnvTmplConfPath,
		EnvLangPath,	EnvPluginsPath,	EnvTmpPath,
		EnvSamplesPath,	EnvPgModelerChPath,	EnvPgModelerCliPath,
		EnvPgModelerSePath,	EnvPgModelerPath
	};

	QString path, filename = search_path + DirSeparator + PgmPathsConfFile + ConfigurationExt;
	QSettings pth_file(filename, QSettings::IniFormat);

	for(auto &var : env_vars)
	{
		path = pth_file.value(var).toString();

		if(!path.isEmpty())
		{
			if(path.startsWith("."))
				path.prepend(search_path + DirSeparator);
		}
		else
			path = getenv(var.toStdString().c_str());

		CustomPaths[var] = QDir::toNativeSeparators(path);
	}
}

void GlobalAttributes::setSearchPath(const QString &search_path)
{
	setCustomPaths(search_path);
	setConfigFilesPaths();

	SchemasRootPath=getPathFromEnv(EnvSchemasPath, SCHEMASDIR, QString("%1/%2").arg(search_path, SchemasDir));
	LanguagesPath=getPathFromEnv(EnvLangPath, LANGDIR, QString("%1/%2").arg(search_path, LanguagesDir));
	SamplesPath=getPathFromEnv(EnvSamplesPath, SAMPLESDIR, QString("%1/%2").arg(search_path, SamplesDir));
	TmplConfigurationPath=getPathFromEnv(EnvTmplConfPath, CONFDIR, QString("%1/%2").arg(search_path, ConfigurationsDir));
	PluginsPath=getPathFromEnv(EnvPluginsPath, PLUGINSDIR, QString("%1/%2").arg(search_path, PluginsDir));

	#if defined(Q_OS_UNIX)
		#if defined(Q_OS_MAC)
			//For MacOSX the crash handler path is fixed (inside bundle)
			PgModelerCHandlerPath=getPathFromEnv(EnvPgModelerChPath, QString("%1/pgmodeler-ch").arg(BINDIR), QString("%1/pgmodeler-ch").arg(search_path));
		#else
			PgModelerCHandlerPath=getPathFromEnv(EnvPgModelerChPath, QString("%1/pgmodeler-ch").arg(PRIVATEBINDIR), QString("%1/pgmodeler-ch").arg(search_path));
		#endif

		PgModelerCLIPath=getPathFromEnv(EnvPgModelerCliPath, QString("%1/pgmodeler-cli").arg(BINDIR), QString("%1/pgmodeler-cli").arg(search_path));
		PgModelerAppPath=getPathFromEnv(EnvPgModelerPath, QString("%1/pgmodeler").arg(BINDIR), QString("%1/pgmodeler").arg(search_path));
		PgModelerSchemaEditorPath=getPathFromEnv(EnvPgModelerSePath, QString("%1/pgmodeler-se").arg(BINDIR), QString("%1/pgmodeler-sc").arg(search_path));

	#else
		PgModelerCHandlerPath=getPathFromEnv(EnvPgModelerChPath, QString("%1\\pgmodeler-ch.exe").arg(PRIVATEBINDIR), QString("%1\\pgmodeler-ch.exe").arg(search_path));
		PgModelerCLIPath=getPathFromEnv(EnvPgModelerCliPath, QString("%1\\pgmodeler-cli.exe").arg(PRIVATEBINDIR), QString("%1\\pgmodeler-cli.exe").arg(search_path));
		PgModelerAppPath=getPathFromEnv(EnvPgModelerPath, QString("%1\\pgmodeler.exe").arg(BINDIR), QString("%1\\pgmodeler.exe").arg(search_path));
		PgModelerSchemaEditorPath=getPathFromEnv(EnvPgModelerSePath, QString("%1/pgmodeler-se.exe").arg(BINDIR), QString("%1/pgmodeler-sc.exe").arg(search_path));
#endif
}

void GlobalAttributes::init(const QString &search_path, bool apply_ui_factor)
{
	QFileInfo fi(search_path);
	GlobalAttributes::setSearchPath(fi.isDir() ? search_path : fi.absolutePath());

	if(apply_ui_factor)
	{
		QString scale = GlobalAttributes::getConfigParamFromFile("custom-scale", GlobalAttributes::AppearanceConf);

		if(scale.toDouble() > 0)
			qputenv("QT_SCALE_FACTOR", scale.toUtf8());
	}
}
