/*
# Projeto: Modelador de Banco de Dados PostgreSQL (pgsqlDBM)
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

/**
\ingroup libutils
\class PgModelerPlugin
\brief Implements the basic operations to create third party plugins based upon shared libraries.
*/

#ifndef PGMODELER_PLUGIN_H
#define PGMODELER_PLUGIN_H

#include "utilsglobal.h"
#include <QString>

class __libutils PgModelerPlugin {
	private:
		QString libname,

		plugin_name;

		//! \brief Defines the name of the library from where the plugin is being loaded
		void setLibraryName(const QString &lib);

		//! \brief Defines the name of plugin itself. In practical terms, it's the plugin's root folder name
		void setPluginName(const QString &name);

		/*! \brief This method returns a full path to a file using the plugin's name as the root folder.
		 * If both subdir and filename are empty, only the root path is returned [root-path]/[plugin-name]
		 * If subdir is empty and filename not then a path in the format [root-path]/[plugin-name]/filename is returned.
		 * If both subdir and filename are set then a path in the format [root-path]/[plugin-name]/subdir/filename is returned. */
		QString getPluginFilePath(const QString &root_path, const QString &subdir, const QString &filename) const;

	public:
		PgModelerPlugin();

		virtual ~PgModelerPlugin();

		//! \brief Returns the name of the library of the plugin
		QString getLibraryName() const;

		//! \brief Returns the name of the plugin
		QString getPluginName() const;

		/*! \brief This method mimics the behavior of GlobalAttributes::getConfigurationFilePath
		 * returning the full path to a file inside a subdirectory in the plugin's root directory.
		 * If both subdir and filename are empty, only the full path to the plugin's root directory is returned.
		 * If subdir is empty and filename not then a path in the format [plugin-root]/filename is returned.
		 * If both subdir and filename are set then a path in the format [plugin-root]/subdir/filename is returned. */
		QString getPluginFilePath(const QString &subdir, const QString &filename) const;

		/*! \brief This method mimics the behavior of GlobalAttributes::getTmplConfigurationFilePath
		 * returning the full path to a file inside a subdirectory in the plugin's template confs directory.
		 * If both subdir and filename are empty, only the full path to the plugin's template confs directory is returned.
		 * If subdir is empty and filename not then a path in the format [plugin-tmpl-conf]/filename is returned.
		 * If both subdir and filename are set then a path in the format [plugin-tmpl-conf]/subdir/filename is returned. */
		QString getTmplPluginFilePath(const QString &subdir, const QString &filename) const;

		friend class PluginsConfigWidget;
		friend class PgModelerCliApp;
};

#endif
