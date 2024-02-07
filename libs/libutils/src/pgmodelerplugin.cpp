/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2024 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

#include "pgmodelerplugin.h"
#include "globalattributes.h"

PgModelerPlugin::PgModelerPlugin()
{

}

PgModelerPlugin::~PgModelerPlugin()
{

}

void PgModelerPlugin::setLibraryName(const QString &lib)
{
	libname = lib;
}

void PgModelerPlugin::setPluginName(const QString &name)
{
	plugin_name = name;
}

QString PgModelerPlugin::getLibraryName() const
{
	return libname;
}

QString PgModelerPlugin::getPluginName() const
{
	return plugin_name;
}

QString PgModelerPlugin::getPluginFilePath(const QString &plugin_name, const QString &root_path, const QString &subdir, const QString &filename)
{
	QString file_pth = root_path + GlobalAttributes::DirSeparator + plugin_name;

	if(!subdir.isEmpty())
		file_pth += GlobalAttributes::DirSeparator + subdir;

	if(!filename.isEmpty())
		file_pth += GlobalAttributes::DirSeparator + filename;

	return file_pth;
}

QString PgModelerPlugin::getPluginTmplFilePath(const QString &plugin_name, const QString &subdir, const QString &filename)
{
	return getPluginFilePath(plugin_name, GlobalAttributes::getPluginsPath(), subdir, filename);
}

QString PgModelerPlugin::getPluginFilePath(const QString &root_path, const QString &subdir, const QString &filename) const
{
	return getPluginFilePath(getPluginName(), root_path, subdir, filename);
}

QString PgModelerPlugin::getConfigFilePath(const QString &subdir, const QString &filename) const
{
	return getPluginFilePath(getPluginName(),
													 GlobalAttributes::getConfigurationsPath() +
													 GlobalAttributes::DirSeparator +
													 GlobalAttributes::PluginsDir, subdir, filename);
}

QString PgModelerPlugin::getTmplConfigFilePath(const QString &subdir, const QString &filename) const
{
	return getPluginFilePath(GlobalAttributes::getPluginsPath(), subdir, filename);
}
