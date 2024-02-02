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

#include "pgmodelercliplugin.h"
//#include "globalattributes.h"

PgModelerCliPlugin::PgModelerCliPlugin()
{

}

PgModelerCliPlugin::~PgModelerCliPlugin()
{

}

void PgModelerCliPlugin::setLibraryName(const QString &lib)
{
	libname = lib;
}

void PgModelerCliPlugin::setPluginName(const QString &name)
{
	plugin_name = name;
}

void PgModelerCliPlugin::initPlugin(PgModelerCliApp *app)
{
	cli_app = app;
}

QString PgModelerCliPlugin::getLibraryName() const
{
	return libname;
}

QString PgModelerCliPlugin::getPluginName() const
{
	return plugin_name;
}

/* QString PgModelerCliPlugin::getPluginFilePath(const QString &root_path, const QString &subdir, const QString &filename) const
{
	QString file_pth = root_path + GlobalAttributes::DirSeparator + getPluginName();

	if(!subdir.isEmpty())
		file_pth += GlobalAttributes::DirSeparator + subdir;

	if(!filename.isEmpty())
		file_pth += GlobalAttributes::DirSeparator + filename;

	return file_pth;
}

QString PgModelerCliPlugin::getPluginFilePath(const QString &subdir, const QString &filename) const
{
	return getPluginFilePath(GlobalAttributes::getConfigurationsPath() +
													 GlobalAttributes::DirSeparator +
													 GlobalAttributes::PluginsDir, subdir, filename);
}

QString PgModelerCliPlugin::getTmplPluginFilePath(const QString &subdir, const QString &filename) const
{
	return getPluginFilePath(GlobalAttributes::getPluginsPath(), subdir, filename);
} */
