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

PgModelerCliPlugin::PgModelerCliPlugin()
{
	cli_app = nullptr;
}

PgModelerCliPlugin::~PgModelerCliPlugin()
{

}

bool PgModelerCliPlugin::isValidOption(const QString &opt) const
{
	static attribs_map short_opts = getShortOptions();

	for(auto &itr : short_opts)
	{
		if(itr.first == opt)
			return true;
	}

	return false;
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
